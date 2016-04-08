#ifndef MENU_TIME_H_
#define MENU_TIME_H_

void menu_time_init(void (*scroll_forward_fp)(volatile menu_choice_t*), void (*scroll_backward_fp)(volatile menu_choice_t *),
		void (*confirm_fp)(volatile menu_system_t *, volatile menu_choice_t *), void (*reject_fp)(volatile menu_system_t *, volatile menu_choice_t *));

extern volatile menu_system_t time_menu;


#endif /* MENU_TIME_H_ */
