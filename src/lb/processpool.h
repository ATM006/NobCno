#ifndef _PROSESSPOOL_H
#define _PROSESSPOOL_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <list>
#include <signal.h>
#include <unistd.h>

#include "struct.h"

#define MAX_EVENTS 1024
#define BUFF_SIZE 128

using namespace std;

extern list<Data> conf_list;
extern list<Map> m;
list<User> user;

class process
{
	public:
		process():m_pid(-1){}
	public:
		pid_t m_pid;
		int m_pipefd[2];

};

class processpool
{
	private:
		processpool(int listenfd,int process_num = 8);
	public:
		static processpool* create(int listenfd,int process_num = 8)
		{
			if(!m_instance)
			{
				m_instance = new processpool(listenfd,process_num);
			}
			return m_instance;
		}

		~processpool()
		{
			delete [] m_sub_process;
		}

		void run();
	private:
		void setup_sig_pipe();
		void run_child();
		void run_parent();

	private:
		static const int MAX_PROCESS_NUMBER = 16;
		static const int USER_PER_PROCESS = 65536;
		static const int MAX_EVENT_NUMBER = 10000;
	   int m_process_number;
		int m_idx;
		int m_epollfd;
		int m_listenfd;
		int m_stop;
		process* m_sub_process;
		static processpool* m_instance;

};

processpool* processpool::m_instance = NULL;


static int sig_pipefd[2];

static int setnonblocking( int fd )
{
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}

static void addfd( int epollfd, int fd )
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
    setnonblocking( fd );
}

static void removefd( int epollfd, int fd )
{
    epoll_ctl( epollfd, EPOLL_CTL_DEL, fd, 0 );
    close( fd );
}

static void sig_handler( int sig )
{
    int save_errno = errno;
    int msg = sig;
    send( sig_pipefd[1], ( char* )&msg, 1, 0 );
    errno = save_errno;
}

static void addsig( int sig, void( handler )(int), bool restart = true )
{
    struct sigaction sa;
    memset( &sa, '\0', sizeof( sa ) );
    sa.sa_handler = handler;
    if( restart )
    {
        sa.sa_flags |= SA_RESTART;
    }
    sigfillset( &sa.sa_mask );
    assert( sigaction( sig, &sa, NULL ) != -1 );
}


processpool::processpool(int listenfd,int process_number )
	:m_listenfd(listenfd),m_process_number(process_number),
	m_idx(-1),m_stop(false)
{
	 assert( ( process_number > 0 ) && ( process_number <= MAX_PROCESS_NUMBER ) );

    m_sub_process = new process[ process_number ];
    assert( m_sub_process );

    for( int i = 0; i < process_number; ++i )
    {
        int ret = socketpair( PF_UNIX, SOCK_STREAM, 0, m_sub_process[i].m_pipefd );
        assert( ret == 0 );

        m_sub_process[i].m_pid = fork();
        assert( m_sub_process[i].m_pid >= 0 );
        if( m_sub_process[i].m_pid > 0 )
        {
            close( m_sub_process[i].m_pipefd[1] );
            continue;
        }
        else
        {
            close( m_sub_process[i].m_pipefd[0] );
            m_idx = i;
            break;
        }
    }
}
void processpool::run()
{
	if(m_idx != -1)
	{
		run_child();
		return;
	}
	run_parent();
}

void processpool::setup_sig_pipe()
{
	 m_epollfd = epoll_create( 5 );
    assert( m_epollfd != -1 );

    int ret = socketpair( PF_UNIX, SOCK_STREAM, 0, sig_pipefd );
    assert( ret != -1 );

    setnonblocking( sig_pipefd[1] );
    addfd( m_epollfd, sig_pipefd[0] );

    addsig( SIGCHLD, sig_handler );
    addsig( SIGTERM, sig_handler );
    addsig( SIGINT, sig_handler );
    addsig( SIGPIPE, SIG_IGN );
}

void processpool::run_child()
{
	cout<<"run_child"<<endl;
	setup_sig_pipe();
	int pipefd = m_sub_process[m_idx].m_pipefd[1];
	addfd(m_epollfd,pipefd);

	epoll_event events[MAX_EVENT_NUMBER];
	//User *users = new User[USER_PER_PROCESS];
	//assert(users);
	list<Map>::iterator it = m.begin();

	while(!m_stop)
	{
		int num = epoll_wait(m_epollfd,events,MAX_EVENT_NUMBER,-1);
		if((num<0)&&(errno == EINTR))
		{
			cout<<"epoll_wait error"<<endl;
			break;
		}

		for(int i = 0 ; i < num ; ++i)
		{
			int retfd = events[i].data.fd;
			if((retfd == pipefd) && (events[i].events & EPOLLIN))
			{
				int client = 0;
				int ret = recv(retfd,(char*)&client,sizeof(client),0);
				if(((ret <0)&&(errno == EAGAIN))|| ret == 0)
				{
					continue;
				}else
				{
					struct sockaddr_in caddr;
					socklen_t len = sizeof(caddr);
					int c = accept(m_listenfd,(struct sockaddr*)&caddr,&len);
					if(c < 0)
					{
						cout<<"accept error errno:"<<errno<<endl;
						continue;
					}


					addfd(m_epollfd,c);

					User u = {caddr,c,0};
					if(it == m.end())
						it = m.begin();
					u.servfd = it->sock;
					++it;
					user.push_back(u);
					
				}
			}
			/*else if((retfd == sig_pipefd[0])&&(events[i].events & EPOLLET))
			{
			
			}*/
			else if(events[i].events == EPOLLIN)
			{
				char buff[BUFF_SIZE]={0};
				int ret = recv(retfd,buff,BUFF_SIZE,0);
				cout<<" "<<buff<<endl;/* condition */
				if(ret < 0)
					{
						if((errno == EAGAIN)||(errno ==EWOULDBLOCK))
						{
							break;
						}
						close(retfd);
						break;
					}
					else if(ret ==0)
					{
						close(retfd);
					}
					else
					{
						list<User>::iterator it_use =  user.begin();
						for(;it_use != user.end();++it_use)
						{
							if(it_use->connfd == retfd)
							{
								send(it_use->servfd,buff,BUFF_SIZE,0);
								send(it_use->connfd,"YES",3,0);
							}
						}
					}

			}
			else
			{
				continue;
			}
		}

	}
	close(pipefd);
	close(m_epollfd);

}

void processpool::run_parent()
{
	cout<<"run_parent"<<endl;
	setup_sig_pipe();
	addfd(m_epollfd,m_listenfd);
	epoll_event events[MAX_EVENT_NUMBER];
	
	int sub_process_counter = 0;
	int new_conn = 1;

	while(!m_stop)
	{
		int num = epoll_wait(m_epollfd,events,MAX_EVENT_NUMBER,-1);
		if((num < 0)&&(errno == EINTR ) )
		{
			cout<<"epoll error "<<endl;
			break;
		}

		for(int i=0;i<num ;++i)
		{
			int retfd = events[i].data.fd;
			if(retfd == m_listenfd)
			{
				int i = sub_process_counter;
				do {
					if(m_sub_process[i].m_pid != -1)
					{
						break;
					}
					i = (i+1)&m_process_number;
				} while (i!= sub_process_counter);

				if(m_sub_process[i].m_pid == -1)
				{
					m_stop = true;
					break;
				}
				sub_process_counter = (i+1)&m_process_number;
				send(m_sub_process[i].m_pipefd[0],(char*)&new_conn,sizeof(new_conn),0);
				cout<<"send request to child :"<<i<<endl;
			}
			/*else if((retfd == sig_pipefd[0])&&(events[i].events & EPOLLIN))
			{
			
			}*/
			else
			{
				continue;
			}
		
		}
	}

}
#endif 
