#include <string.h>
#include <ctype.h>

#include "utils.h"



// ******************************************************************
//  PRIVATE FUNCTIONS IMPLEMENTATION
// ******************************************************************

static int hex2dec(char c) {
	c = tolower(c);
	if ('0' <= c && c <= '9') return (c-'0');
	else if ('a' <= c && c <= 'f') return (c-'a')+10;
	else return -512;
}



// ******************************************************************
//  PUBLIC FUNCTIONS IMPLEMENTATION
// ******************************************************************

// 0 = OK, false otherwise
int parse_color(char *str, struct rgb_color *c)
{
	if (strlen(str) != 6) {
		return 1;
	}

	c->r = (hex2dec(str[0])*16 + hex2dec(str[1])) / 255.0;
	c->g = (hex2dec(str[2])*16 + hex2dec(str[3])) / 255.0;
	c->b = (hex2dec(str[4])*16 + hex2dec(str[5])) / 255.0;
	return (c->r<0) || (c->g<0) || (c->b<0);
}

