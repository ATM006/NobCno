#include <iostream>
#include "mysql.h"
#include "db.h"
using namespace std;


MYSQL conn;


int main()
{
	connect_db();

	close_db();

}
