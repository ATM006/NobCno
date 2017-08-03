#ifndef _ARTICLE_H
#define _ARTICLE_H

#pragma pack(1)
#include <time.h>

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
	
		
//private:
public:
	long m_id;
	char *m_title;
	char *m_author;
	char *m_description;
	char *m_content;
	time_t m_dateline;
};

#endif
