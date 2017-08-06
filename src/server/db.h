#ifndef DB_H
#define DB_H
#include "article.h"

class Article;

bool connect_db();
void close_db();

void inset(Article *art);
#endif
