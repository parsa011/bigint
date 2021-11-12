#define printl(x) printf("%s\n",x)
#define EOL '\n'
#define BUFSIZE 10

typedef struct {
	int len;		// len of val string
	char *val;		// our big int will store here
	int sign;		// sign of number , just can be -1 and 1
} big_int;

// functions declarations

/*
 * 	get next big int from user and return it in big_int struct style
 */
big_int *get_big_int();
