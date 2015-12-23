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

void console_go(void);

/*
 * static functions
 */

static void execute_command(const char *cmd);



#endif /* CONSOLE_H_ */
