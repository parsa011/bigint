#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <termios.h>
#include <string.h>

#include "definitions.h"

int main() 
{
	big_int first = INIT_BIG_INT, second = INIT_BIG_INT;
	get_big_int(&first);
	get_big_int(&second);
	sub_big_int(&first,&second);
	print_bigint(first);
	printl("");
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
				// asnegative sing of the buf when first char was '-'
				if (is_negative) {
					buf->negative = true;
					goto write_char;
				}
				// check if we need more space for our buf->val
				if (len == bufsize) {
					bufsize += BUFSIZE;
					buf->val = realloc(buf->val,bufsize);
				}
				buf->val[len++] = c;
write_char:
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
	// if one of them was negative , we have to subtract them :DDD , nice right ?
	if ((first->negative && !second->negative) || (!first->negative && second->negative)) {
		sub_big_int(first,second);
		return;
	}
	// carry value for next two char
	int carry = 0;
	int i, j;
	if (first->len < second->len) {
		first->len = second->len;
		first->val = realloc(first->val,first->len);
		// after realloc , move chars all they way down to end of array
		while (first->val[first->len - 1] == NULL)
			shift_right(first->val,first->len,0);
	}
	for (i = first->len - 1,j = second->len - 1;i >= 0 || j >= 0;i--,j--) {
		while (first->val[i] == NULL)
			i--;
		// first int char , convert it to real int
		int f = get_char_as_int(first->val[i]);
		// second int char , if i wasn't out of the second big int len 
		int b = get_char_as_int(second->val[j]);
		// this is the result of current chars sum
		int temp = f + b + carry;
		carry = 0;
		if (temp >= 10) {
			carry = temp == 10 ? 1 : temp / 10;
			temp -= 10;
		}
		//printf("in i %d and j %d,first is %d and second is %d, and we have %d for temp,so carry is: %d\n",i,j,f,b,temp,carry);
		first->val[i] = temp + NUM;
	}
	if (carry != 0) {
		first->len++;
		first->val = realloc(first->val,first->len);
		shift_right(first->val,first->len,0);
		first->val[0] = carry + NUM;
	}
}

void sub_big_int(big_int *first,big_int *second)
{
	// if both was negative
	if (first->negative && second->negative) {
		sum_big_int(first,second);
		return;
	}
}

void shift_right(char *buf,int buflen,int start_index)
{
	char next = 0,temp = buf[start_index];
	for (int i = buflen - 1;i > start_index;i--) {
		buf[i] = buf[i - 1];
		buf[i - 1] = ' ';
	}
}

// if input char (c) is digit , decrease 48 of to convert to real int
int get_char_as_int(char c)
{
	return isdigit(c) ? c - NUM : 0;
}

void print_bigint(big_int buf)
{
	if (buf.negative)
		printf("-");
	printf("%s",buf.val);
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
