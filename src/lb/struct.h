#ifndef _STRUCT_H
#define _STRUCT_H
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



using namespace std;

typedef struct Data
{
	string ip;
	string port;
	bool tag;
}Data;

typedef struct Map
{
	int sock;
	string port;
	//int cock;
}Map;

typedef struct User
{
	struct sockaddr_in address;
	int connfd;
	int servfd;
}User;
#endif
