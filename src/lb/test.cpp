#include <iostream>
#include "processpool.h"

using namespace std;

int main()
{
	int x;
	processpool*  pool = processpool::create(x,6); 
	pool->run();

	return 0;
}
