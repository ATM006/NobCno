#include "server.h"
#include "article.h"
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <cstring>

 #include <stdlib.h>

int server::m_count = 0;

int setnonblocking( int fd )
{
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}

void server::running()
{
        struct epoll_event events[MAXSIZE];
        int nready = 0;
        while(1)
        {
                nready = epoll_wait(this->epfd,events,MAXSIZE,-1);
                if(nready > 0)
                {
                    int i;
                    for(i = 0;i<nready;i++)
                    {
                        int tmpfd = events[i].data.fd;
                        if(tmpfd == m_sfd)//监听
                        {
                            struct sockaddr_in clientaddr;
                            socklen_t c_len = sizeof(clientaddr); 
                            int cfd = accept(tmpfd,(struct sockaddr*)&clientaddr,&c_len);
							setnonblocking(cfd);
                            struct epoll_event event;
                            event.events = EPOLLIN;
                            event.data.fd = cfd;
                            //join tree
							server::m_count+=1;
                            epoll_ctl(this->epfd,EPOLL_CTL_ADD,cfd,&event);
                        }
                        else if(events[i].events && EPOLLIN)
                        {   
                            //char buf[1024] = {0};
                            char buf[100] = {0};
                            int len = MyRecv(tmpfd,buf,sizeof(buf));
							if(len<0)
							{
								std::cout<<"    <"<<__FILE__<<":"<<__LINE__<<"> "<<"MyRecv failed!"<<std::endl;
							}
							int read_size = (int)ntohl(*(int*)buf);
							std::cout<<"    <"<<__FILE__<<":"<<__LINE__<<"> "<<"int read_szie ="<<len<<std::endl;
							len = MyRecv(tmpfd,buf,read_size);
							{
								std::cout<<"    <"<<__FILE__<<":"<<__LINE__<<"> "<<"MyRecv failed!"<<std::endl;
							}

							//std::cout<<strlen(buf)<<" "<<buf<<std::endl; 
							//memset(buf,0,sizeof(buf));                          
									

							 /* if(0 >= len)
                            {
                                epoll_ctl(this->epfd,EPOLL_CTL_DEL,tmpfd,NULL);
                              	server::m_count -= 1;
								 if(server::m_count==0)
								{
									shutdown(tmpfd,SHUT_RDWR);
                            	}
							}*/
                          //  MySend(tmpfd,buf,len);
                        }else
						{
						
						}
                    }
					
						
			
                }
        }
}


server::server(int port):m_port(port),m_sfd(-1)
{
        //创建套接字
        m_sfd = socket(AF_INET,SOCK_STREAM,0);
        int ret = 0;
        if(-1 == m_sfd)
        {
            throw "创建套接字\n";
        }
        //端口复用
        int opt = 1;
        setsockopt(m_sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));


        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(m_port);
        serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
        //绑定
        ret = bind(m_sfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
        if(-1 == ret)
        {
            throw "绑定失败\n";
        }
        //监听
        ret = listen(m_sfd,128);
        if(-1 == ret)
        {
            throw "监听失败";
        }
        //创建树根
        this->epfd = epoll_create(MAXSIZE);
        if(-1 == this->epfd)
        {
            throw "epoll_create is wrong";
        }
        struct epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = m_sfd;
        //加入树中
        epoll_ctl(this->epfd,EPOLLIN,m_sfd,&event);
}


server::~server()
{
	
   shutdown(this->m_sfd, SHUT_RDWR);
}

int server::MySend(int iSock,char *pchBuf,size_t tLen)
{
	int iThisSend;
	unsigned int iSended = 0;
	if(0 == tLen)
		return 0;
	while(iSended<tLen)
	{
		do{
			iThisSend  = send(iSock,pchBuf,tLen-iSended,0);
		}while((iThisSend<0)&&(errno == EINTR));
		if(iThisSend<0)
			return iSended;
		iSended += iThisSend;
		pchBuf += iThisSend;
	}
	return tLen;
}

int server::MyRecv( int iSock, char * pchBuf, size_t tCount){
        size_t tBytesRead=0;
        int iThisRead;
        while(tBytesRead < tCount){

               do{
                     iThisRead = read(iSock, pchBuf+tBytesRead, tCount-tBytesRead);
              } while((iThisRead< 0) && (errno==EINTR));
               if(iThisRead < 0){
                      return iThisRead;
              }
               else if (iThisRead == 0)
                     return tBytesRead;
              tBytesRead += iThisRead;
              pchBuf += iThisRead;
       }
}
