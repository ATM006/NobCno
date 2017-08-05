#ifndef _ARTICLE_H
#define _ARTICLE_H

#pragma pack(1)
#include <time.h>

#include <arpa/inet.h>


const int ADD =  1;  
const int DEL = 2;
const int MOD = 3;
const int SEA = 4;


//article class
class Article
{
public:
	Article();
	~Article();
	void add_article();
	void del_article();
	void modify_article();
	void download_article();
	void search_article();
	void un_package(char* buf,int len,int &op);	
	void mk_package(char* &buff,int &len,char* &tmp,int op);		
//private:
public:
	int m_id;
	char *m_title;
	char *m_author;
	char *m_description;
	char *m_content;
	time_t m_dateline;
};

#endif
