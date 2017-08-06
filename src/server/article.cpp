#include "article.h"
#include "../log/log.h"
#include "db.h"
#include <cstring>
#include <iostream>


extern void insert(Article *art);

Article::Article():m_id(-1)
{
	m_title = new char[100];
	memset(m_title,0,sizeof(m_title));	
	m_author = new char[50];
	memset(m_author,0,sizeof(m_author));
	m_description = new char[255];
	memset(m_description,0,sizeof(m_description));
	m_content = NULL;
	m_dateline = 0;
}



Article::~Article()
{
	m_id = -1;
	delete [] m_title;
	delete [] m_author;
	delete [] m_description;
	delete [] m_content;
	m_dateline = -1;
}

void Article::add_article()
{
	log("add_article()");
	insert(this);	
}
void Article::del_article(){}
void Article::modify_article(){}
void Article::download_article(){}
void Article::search_article(){}

void Article::mk_package(char* &pBuff,int &iLen,char* &tmp,int op)
{
	log("mk_package()");
	*(int*)(pBuff+iLen)= htonl(op);
    iLen+=sizeof(int);
	std::cout<<"op = "<<op<<std::endl;
    *(int*)(pBuff+iLen) = htonl(m_id);
    iLen += sizeof(int);
    memcpy(pBuff+iLen,m_title,100);
    iLen += 100;
	std::cout<<"title = "<<m_title<<std::endl;
    memcpy(pBuff+iLen,m_author,50);
    iLen += 50; 
    memcpy(pBuff+iLen,m_description,255);
    iLen += 255;
   	int tmp_len = strlen(m_content);
	//std::cout<<"tmp_len:"<<tmp_len<<std::endl;
	*(int*)(pBuff+iLen) = htonl(tmp_len);
	iLen += sizeof(int);
	memcpy(pBuff+iLen,m_content,sizeof(m_content));
    iLen += (sizeof(tmp));
    *(time_t*)(pBuff+iLen) = htonl(m_dateline);
    iLen += (sizeof(time_t));

}


void Article::un_package(char* buf,int iLen,int &op)
{


	log("un_package()");
	int len = 0;
	memcpy(&op,buf+len,sizeof(int));	//1
	len += sizeof(int);
	op = ntohl(op);
	std::cout<<op<<std::endl;
	memcpy(&m_id,buf+len,sizeof(int));	//2
	len += sizeof(int);
	m_id = ntohl(m_id);
	std::cout<<m_id<<std::endl;
	memcpy(m_title,buf+len,sizeof(100));//3
	len += 100;
	std::cout<<m_title<<std::endl;
	memcpy(m_author,buf+len,sizeof(50));
	len += 50;
	std::cout<<m_author<<std::endl;
	memcpy(m_description,buf+len,sizeof(255));
	len += 255;
	std::cout<<m_description<<std::endl;
	int tmp,tmp_len ;
	memcpy(&tmp,buf+len,sizeof(int));
	len += sizeof(int);
	tmp_len = ntohl(tmp);
	std::cout<<tmp_len<<std::endl;
	m_content = new char[tmp_len+1];
	memcpy(m_content,buf+len,tmp_len);
	m_content[tmp_len]=0;
	len += tmp_len;
	std::cout<<m_content<<std::endl;
	time_t t1,t2;
	memcpy(&t1,buf+len,sizeof(time_t));
	len+=sizeof(time_t);
	t2 = ntohl(t1);
	std::cout<<m_dateline<<" "<<sizeof(time_t)<<std::endl;
}
