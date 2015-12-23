#ifndef CONSOLE_H_
#define CONSOLE_H_

typedef void(*function_pointer)(void);

typedef struct
{
	const char *name;
	function_pointer execute;
	const char *help;
} command;


void console_execute_command(const char *cmd);



#endif /* CONSOLE_H_ */
