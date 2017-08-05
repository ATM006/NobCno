#include <iostream>
#include <string>
#include <list>
#include <iterator>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <map>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>	//关闭套接字
#include <string.h>
#include "struct.h"

using namespace std;
/*typedef struct Data
{
	string ip;
	string port;
	bool tag;
}Data;
*/

list<Data> conf_list;
list<int> so_list;
list<Map>  m;

void read_conf()
{
	fstream f;
	f.open("./conf.xml");
	Data data;
	while((f>>data.ip)&&(f>>data.port))
	{
		data.tag=0;
		conf_list.push_back(data);
	}

	f.close();
}


void conn()
{
	read_conf();
	
	list<Data>::iterator it = conf_list.begin();
	int i=0;
	for(;it != conf_list.end();++it ,++i)
	{
		//cout<<it->port<<"   "<<it->ip<<endl;
		struct sockaddr_in caddr;
		bzero(&caddr,sizeof(caddr));
		caddr.sin_family = AF_INET;
		caddr.sin_addr.s_addr = inet_addr((it->ip).c_str());
		caddr.sin_port = htons(atoi((it->port).c_str()));
		
		cout<<(it->ip).c_str()<<" "<<atoi((it->port).c_str())<<endl;//×××
		
		int socketfd = socket(AF_INET,SOCK_STREAM,0);
		assert( socketfd != -1 );
	
		int ret = connect(socketfd,(struct sockaddr*)&caddr,sizeof(caddr));
		if(ret == -1 )
		{
			cout<<"connect error++ "<<endl;		
		}
		else
		{
			//so_list.push_back(socketfd);
			Map node;
			node.sock = socketfd;
			node.port = it->port;
			//node.port = 0;
	   	m.push_back(node);
		}
	}
	
}

/*int main()
{
	//read_conf();
	conn();

	list<Map>::iterator it = m.begin();
	for(;it != m.end();++it)
	{
		cout<<it->sock<< " "<<it->port<<endl;
	}

	return 0;
}
*/

