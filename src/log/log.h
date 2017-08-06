#ifndef _LOG_H
#define _LOG_H
#include <iostream>

#define log(msg)	std::cout<<"    < log > ||"<<__FILE__<<":"<<__LINE__<<"#"<<msg<<std::endl;

#endif
