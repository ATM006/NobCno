#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "define.h"
#include "article.h"

/*
const int ADD =  1;
const int DEL = 2;
const int MOD = 3;
const int SEA = 4;
*/

#define DEFAULT_PORT 6501
int main( int argc, char * argv[]){
    int connfd = 0;
    int cLen = 0;
    struct sockaddr_in client;
    if(argc < 2){
        printf(" Uasge: clientent [server IP address]\n");
        return -1;
    }
    client.sin_family = AF_INET;
    client.sin_port = htons(DEFAULT_PORT);
    client.sin_addr.s_addr = inet_addr(argv[1]);
    connfd = socket(AF_INET, SOCK_STREAM, 0);
    if(connfd < 0){
        printf("socket() failure!\n" );
        return -1;
    }
    if(connect(connfd, (struct sockaddr*)&client, sizeof(client)) < 0){
        printf("connect() failure!\n" );
        return -1;
    }

	/*
    Pkg mypkg;
    mypkg.head.num=1;
    mypkg.head.index=10001;
    mypkg.content.sex='m';
    mypkg.content.score=90;
    char * temp="guangzhou and shanghai";
    strncpy(mypkg.content.address,temp,sizeof(mypkg.content.address));
    mypkg.content.age=18;
	*/
	Article art;
	art.m_id = 1;
	strcpy(art.m_title,"1234");
	strcpy(art.m_author,"5678");
	strcpy(art.m_description,"abcd");
	char *tmp = "exxxx";
	art.m_content = new char[sizeof(tmp)+1];
	strcpy(art.m_content,tmp);
	art.m_dateline = 1501345108;

	ssize_t writeLen;
    //int tLen=sizeof(art);
    //printf("tLen:%d\n" ,tLen);
    int iLen=0;
    char * pBuff= new char [1000];
    //*(int*)(pBuff+iLen)= htonl(iLen);
    //iLen+=sizeof( int);
	
   
/*	 *(int*)(pBuff+iLen)= htonl(ADD);
	iLen+=sizeof(ADD);
	*(long*)(pBuff+iLen) = htonl(art.m_id);
	iLen += sizeof(long);
	memcpy(pBuff+iLen,art.m_title,100);
	iLen += 100;
	memcpy(pBuff+iLen,art.m_author,50);
 	iLen += 50;
	memcpy(pBuff+iLen,art.m_description,255);
 	iLen += 255;
	memcpy(pBuff+iLen,art.m_content,sizeof(tmp));
 	iLen += (sizeof(tmp));
	*(int*)(pBuff+iLen) = htonl(art.m_dateline);
	iLen += (sizeof(time_t));
*/	
	art.mk_package(pBuff,iLen,tmp,1);
	/*
	*(int*)(pBuff+iLen)= htonl(mypkg.head.num);
    iLen+=sizeof( int);
    *(int*)(pBuff+iLen)= htonl(mypkg.head.index);
    iLen+=sizeof( int);
    memcpy(pBuff+iLen,&mypkg.content.sex,sizeof( char));
    iLen+=sizeof( char);
    *(int*)(pBuff+iLen)= htonl(mypkg.content.score);
    iLen+=sizeof( int);
    memcpy(pBuff+iLen,mypkg.content.address,sizeof(mypkg.content.address));
    iLen+=(sizeof(mypkg.content.address));
    *(int*)(pBuff+iLen)= htonl(mypkg.content.age);
    iLen+=sizeof( int);
	*/
    writeLen= MySend(connfd, pBuff, iLen);
    if (writeLen <= 0) {
       printf("write failed\n" );
       close(connfd);
       return 0;
    }
    else{
       printf("write sucess, writelen :%d, iLen:%d, pBuff: %s\n",writeLen,iLen,pBuff);
    }

while(1)
;
    close(connfd);
	
    return 0;
}
