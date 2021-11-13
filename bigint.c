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
	sum_big_int(&first,&second);
	//printf("%s : %d\n",first.val,first.len);
	printl(first.val);
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

void sum_big_int(big_int *first,big_int *second)
{
	// carry value for next two char
	int carry = 0;
	if (first->len < second->len) {
		first->len = second->len;
		first->val = realloc(first->val,first->len);
	}
	for (int i = first->len - 1,j = second->len - 1;i >= 0 && j >= 0;i--,j--) {
		// this is the result of current chars sum
		int temp = 0;
		// first int char , convert it to real int
		int f = first->val[i] - NUM;
		if (f < 0)
			f = 0;
		// second int char , if i wasn't out of the second big int len 
		int b = 0;
		b = second->val[j] - NUM;
		temp = f + b + carry;
		carry = 0;
		if (temp >= 10) {
			carry = temp == 10 ? 1 : temp / 10;
			temp -= 10;
		}
		printf("in i %d and j %d, first is %d and second is %d, and we have %d for temp,so carry is: %d\n",i,j,f,b,temp,carry);
		first->val[i] = temp + NUM;
	}
}

void sub_big_int(big_int *first,big_int *second)
{

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
