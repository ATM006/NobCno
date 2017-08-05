#include <iostream>
#include "mysql.h"
#include "server.h"
#include "db.h"

using namespace std;

MYSQL conn;

int main(int argc, char *argv[])
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
return 0;
}
