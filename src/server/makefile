all:NobCno

NobCno:db.cpp  server.cpp article.cpp main.cpp
	 g++ -Wall -std=c++11  -D_GLIBCXX_USE_CXX11_ABI=0  -o  $@ $^   -lmysqlclient -I /usr/include/mysql
