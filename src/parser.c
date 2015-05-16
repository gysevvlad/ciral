#include "parser.h"

int PARSE_LINE = 1;

static char get_next_char( FILE *fp );
static char* is_int( char *str );
static char* is_uint( char *str );
static char* is_char_eq( char *str, char c );
static int is_end( char *str );

/* 
Pattern:
   1 [VRICLD]<uint>
   2 <uint>
   3 <float>
*/
int token_check( char *token, int pattern ) {
	int status = 0;
	char c;

#define is_elem( c ) \
	(c == 'V' || c == 'R' || c == 'I' ||\
	 c == 'C' || c == 'L' || c == 'D')

	switch (pattern) {
		case 1:
			c = *token++;
			if (is_elem( c ) && is_end(is_uint( token )))
				status = 1;
			break;

		case 2:
			if (is_end(is_uint( token )))
				status = 1;
			break;

		case 3:
			if (is_end(is_uint(is_char_eq(is_int( token ), '.'))))
				status = 1;
			break;
	}
	return status;
}

size_t token_get_next( FILE *fp, char *buf, size_t size ) {
	size_t i = 0;
	char tmp;
	while ( tmp = get_next_char( fp ), 
	        tmp != ' ' && tmp != EOF && i < size-1 )
		i++, *buf++ = tmp;
	*buf = '\0';
	return i;
}

char get_next_char( FILE *fp ) {
	static int prev_space = -1;
	char c;
	do {
		c = (char)fgetc( fp );
		if ( c == '\n' ) 
			PARSE_LINE++;
		if ( c == '\t' || c == '\n' )
			c = ' ';
		if ( c == ' ' && prev_space == 0 ) {
			prev_space = 1;
			goto out;
		}
		if ( c == '*' ) {
			while ( c = (char)fgetc( fp ), c != '\n' && c != EOF );
			if ( c == EOF)
				goto out;
			if ( c == '\n' )
				c = ' ', PARSE_LINE++;
		}
	} while ( c == ' ' && prev_space != 0 );
	prev_space = 0;
out:
	return c;
}

char* is_int( char *str ) {
	char c = *str++,
	     sign = 0;
	if (c == '+' || c == '-')
		c = *str++, sign = 1;
	if ( c == '0' )
		return str;
	if ( c < '1' || c > '9')
		return (sign)?str-2:str-1;
	while ( c >= '0' && c <= '9' ) c = *str++;
	return str-1;
}

char* is_uint( char *str ) {
	char c = *str++;
	if ( c == '0' )
		return str;
	if ( c < '1' && c > '9')
		return str-1;
	while ( c >= '0' && c <= '9' ) c = *str++;
	return str-1;
}

char* is_char_eq( char *str, char c ) {
	if ( *str == c )
		str++;
	return str;
}

int is_end( char *str ) {
	return (*str == '\0')? 1: 0;		
}