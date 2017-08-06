#include "db.h"
#include "mysql.h"
#include "../log/log.h"

#include <iostream>

extern MYSQL conn;

bool connect_db()
{
	mysql_init(&conn);
	 if(mysql_real_connect(&conn,"localhost","root","11111111","article",0,NULL,CLIENT_FOUND_ROWS)) //"root":数据库管理员 "":root密码 "test":数据库的名字
	{
 		//std::cout<<"	<"<<__FILE__<<":"<<__LINE__<<"> "<<"connect database successed"<<std::endl;
		log("connect database successed");
		return true;
	}
	return false;
}

void close_db()
{
	mysql_close(&conn);
	log("closed DB");
}

void insert(Article *art)
{
	log("insert()");
 	char *title = art->m_title;
    char *author = art->m_author;
	char *description = art->m_author;
	char *content = art->m_content;
	long int dateline = art->m_dateline;
    char sql[1024]={0};
    sprintf(sql,"INSERT INTO `article` ( `title`, `author`, `description`, `content`, `dateline`) VALUES ('%s', '%s', '%s', '%s','%ld');",title,author,description,content,dateline);

    mysql_query( &conn, sql);

}
