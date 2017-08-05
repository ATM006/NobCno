#ifndef _SERVER_H
#define _SERVER_H
 
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXSIZE 1024
class server
{
    private:
		static int m_count;
        int m_port;
        int m_sfd;
        struct sockaddr_in serveraddr;
        int epfd;
	public:	
		int MySend(int iSock,char *pchBuf,size_t tLen);
		int MyRecv(int iSock,char *&pchBuf,size_t tCount);
    public:
        server(int port = 6501);
        ~server();
        //启动
        void running();

};

#endif
