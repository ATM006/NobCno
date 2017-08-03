#include <iostream>
#include "mysql.h"
#include "db.h"
#include "server.h"

using namespace std;


MYSQL conn;


int main()
{
	connect_db();


	 try
	    {
		server *s = new server(6501);
		s->running();
		delete s;
	    }
	    catch(const char *str)
	    {
		cout << str << endl;
	    }


	close_db();

}


