#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <termios.h>
#include <string.h>

#include "definitions.h"

int main() 
{
	big_int first, second;
	get_big_int(&first);
	get_big_int(&second);
	printl(first.val);
	printl(second.val);
	return 0;
}

// functions declarations

/*
 *  get big int from user , and prevent of input letters and non digit characters , excecpt '-' for first char
 */
void get_big_int(big_int *buf)
{
	buf->val = malloc(BUFSIZE);
	int c,len = 0;
	int bufsize = BUFSIZE;
	while (true) {
		c = getch(false);
		// if taht was backspace key , move cursor to left , and remove last char of buf->val by decreasing
		// 	val->len
		if (c == KEY_BS) {
			if (len != 0)
				len--;
			delete_left_char();
			continue;
		}
		if (c == EOL || c == EOF) {
			// if that was '\n' or EOF , close the string
			buf->val[len] = '\0';
			break;
		}else {
			// check if c is digit , or its the first char that user inputed and its '-' (for negative numbers)
			bool is_negative = len == 0 && c == '-';
			if (isdigit(c) || is_negative) {
				// assign sing of the buf when first char was '-'
				if (is_negative)
					buf->sign = -1;
				// check if we need more space for our buf->val
				if (len == bufsize) {
					bufsize += BUFSIZE;
					buf->val = realloc(buf->val,bufsize);
				}
				buf->val[len++] = c;
				printf("%c",c);
			}
		}
	}
	buf->len = len;
	// insert new line
	printl("");
}

// terminal stuff

static int getch(bool write)
{
	enter_raw_mode();
	int c = getchar();
	exit_raw_mode();
	if (write)
		printf("%c",c);
	return c;
}

void enter_raw_mode()
{
	tcgetattr(0, &oterm);
	memcpy(&term, &oterm, sizeof(term));
	term.c_lflag &= ~(ICANON | ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &term);
	atexit(exit_raw_mode);
}

void exit_raw_mode()
{
	tcsetattr(0, TCSANOW, &oterm);
}

void delete_left_char()
{
	// tricky move for clear last char of screen , move left , write SPACE , and again move left :)
	CURSOR_LEFT(1);
	printf(" ");
	CURSOR_LEFT(1);
}
