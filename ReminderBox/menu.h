#ifndef MENU_H_
#define MENU_H_

typedef int menu_choice_t;


typedef struct menu_system_t
{
	menu_choice_t current_choice;

	void (*confirm)(volatile struct menu_system_t *menu, volatile menu_choice_t *menu_item);
	void (*reject)(volatile struct menu_system_t *menu, volatile menu_choice_t *menu_item);
	void (*scroll_menu_forward)(volatile menu_choice_t *menu_item);
	void (*scroll_menu_backward)(volatile menu_choice_t *menu_item);
} menu_system_t;

#define TOP_MENU_SIZE 5
typedef enum
{
	DISPLAY_TIME = 0,
	CHECK_DUE_DATES = 1,
	PAY_BILL = 2,
	ENTER_NEW_DATE = 3,
	SET_TIME = 4
} top_menu_e;

typedef enum
{
	CHOICE_DISPLAY_TIME = 5
} choice_time_e;


#endif /* MENU_H_ */
