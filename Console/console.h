#ifndef CONSOLE_H_
#define CONSOLE_H_



/*
 * defines
 */

#define INPUT_LENGTH 20


/*
 * typedefs
 */

typedef void(*function_pointer)(void);
typedef struct
{
	const char *name;
	function_pointer execute;
	const char *help;
} command;

/*
 * API functions
 */

const char * console_get_help_str(void);
int console_go(void);//bool

/*
 * static functions
 */

static int execute_command(const char *cmd);//bool



#endif /* CONSOLE_H_ */
