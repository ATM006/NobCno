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
#include <unistd.h>

#include "processpool.h"
//#include "struct.h"

#define MAX_EVENTS 1024
#define BUFF_SIZE 128
using namespace std;

//extern list<Data> conf_list;
//extern list<Map> m;
//list<User> user;

/*
int setnonblocking( int fd )
{
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}

void addfd( int epollfd, int fd )
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
   // event.events = EPOLLIN;
    epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
    setnonblocking( fd );
}
*/
extern void conn();
 
int main()
{
	cout<<"server start.."<<endl;

	conn();

	int use_count = 0;

	struct sockaddr_in saddr;
	bzero(&saddr,sizeof(saddr));

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(6510);

	int s_socket = socket(AF_INET,SOCK_STREAM,0);
	assert(s_socket != -1);

	int ret = bind(s_socket,(struct sockaddr*)&saddr,sizeof(saddr));
	if( ret == -1)
	{
		printf("bind error\n");
		exit(-1);
	}

	listen(s_socket,5);


   processpool* pool =  processpool::create(s_socket);
	if(pool)
	{
		pool->run();
		delete pool;
	}

#if 0
	epoll_event events[MAX_EVENTS];
	int epollfd = epoll_create(5);
	assert(epollfd !=-1);
	addfd(epollfd,s_socket);

	list<Map>::iterator it = m.begin();
	
	while(1)
	{
		int num = epoll_wait(epollfd,events,MAX_EVENTS,-1);
		if(num <0 )
		{
			cout<<"epoll_wait error"<<endl;
			break;
		}

		
		for(int i=0;i<num;++i)
		{
			int retfd = events[i].data.fd;
			if(retfd == s_socket)
			{
				struct sockaddr_in caddr;

				cout<<"accept.."<<endl;
	
				socklen_t len = sizeof(caddr);
				int c = accept(s_socket,(struct sockaddr*)&caddr,&len);
				if(c <0)
				{
					cout<<"accept error"<<endl;
				}
				
				User* u = new User;
				u->address = caddr;
				u->connfd = c;
				if(it == m.end())
					it = m.begin();
				u->servfd = it->sock;
				++it;
				user.push_back(*u);
				delete u;
	
				addfd(epollfd,c);
				use_count++;
				cout<<" Thare are "<<use_count<<" clients"<<endl;
			}
			else if(events[i].events & EPOLLIN)
			{
				char buff[BUFF_SIZE]={0}; //置为全局buff
					
				while(1)
				{
					memset(buff,0,BUFF_SIZE);
					int ret = recv(retfd,buff,BUFF_SIZE,0);
				//	int ret = recv(it->sock,buff,BUFF_SIZE,0);
					cout<<" "<<buff<<endl;
					
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
			}
			else
			{
				cout<<"somthing"<<endl;
			}
		
		}

		
	}
#endif

	close(s_socket);
	exit(0);

}
