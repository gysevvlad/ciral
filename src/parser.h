#ifndef PARCE_H
#define PARSE_H

#include <stdio.h>
#include <string.h>

/* 
Pattern:
   1 [VRICLD]<int>
   2 <int>
   3 <float>
*/
int token_check( char *token, int pattern );

size_t token_get_next( FILE *fp, char *buf, size_t size );

extern int PARSE_LINE;

#endif