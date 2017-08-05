#this is a makefile for loadbalance
edit:conn.o main.o  
	g++ -o edit conn.o main.o 
main.o:main.cpp processpool.h
	g++ -c main.cpp 
conn.o:conn.cpp struct.h
	g++ -c conn.cpp



clean:
	rm *.o 
