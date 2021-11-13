#define printl(x) printf("%s\n",x)
#define EOL '\n'
#define BUFSIZE 10

#define CURSOR_LEFT(x) printf("\033[%dD", (x))
#define CURSOR_RIGHT(x) printf("\033[%dC", (x))
#define KEY_BS 0x7F // backslash key code

typedef struct {
	int len;		// len of val string
	char *val;		// our big int will store here
	int sign;		// sign of number , just can be -1 and 1
} big_int;

// ----------------------
//
// functions declarations
//
// ----------------------

/*
 * 	get next big int from user and return it in big_int struct style
 */
void get_big_int(big_int *);


// terminal stuff
static struct termios term, oterm;
void enter_raw_mode();
void exit_raw_mode();
static int getch(bool); // bool write into screen or no
void delete_left_char();
