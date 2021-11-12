#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "definitions.h"

int main() 
{
	big_int first;
	get_big_int(&first);
	printl(first.val);
	return 0;
}

// functions declarations

void get_big_int(big_int *buf)
{
	buf->val = malloc(BUFSIZE);
	int c,len,bufsize = BUFSIZE;
	while (true) {
		c = getchar();
		if (c == EOL || c == EOF) {
			buf->val[len] = '\0';
			break;
		}else {
			// check if c is digit , or its the first char that user inputed and its '-' (for negative numbers)
			bool is_negative = len == 0 && c == '-';
			if (isdigit(c) || is_negative) {
				if (is_negative)
					buf->sign = -1;
				// check if we need more space for our buf->val
				if (len == bufsize) {
					bufsize += BUFSIZE;
					buf->val = realloc(buf->val,bufsize);
				}
				buf->val[len++] = c;
			}
		}
	}
	buf->len = len;
}
