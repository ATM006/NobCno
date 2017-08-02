#include "db.h"
#include "mysql.h"
#include <iostream>

extern MYSQL conn;

bool connect_db()
{
	mysql_init(&conn);
	 if(mysql_real_connect(&conn,"localhost","root","11111111","article",0,NULL,CLIENT_FOUND_ROWS)) //"root":数据库管理员 "":root密码 "test":数据库的名字
	{
 		std::cout<<"	<"<<__FILE__<<":"<<__LINE__<<"> "<<"connect database successed"<<std::endl;
		return true;
	}
	return false;
}

void close_db()
{
	mysql_close(&conn);
}
