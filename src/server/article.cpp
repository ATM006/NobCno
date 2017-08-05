#include "article.h"
#include <cstring>
#include <iostream>

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

void Article::add_article(){}
void Article::del_article(){}
void Article::modify_article(){}
void Article::download_article(){}
void Article::search_article(){}

void Article::un_package(char* buf,int len)
{
	std::cout<<"un_package()"<<std::endl;
}
