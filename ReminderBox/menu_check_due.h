#ifndef MENU_CHECK_DUE_H_
#define MENU_CHECK_DUE_H_

void menu_check_due_init(void (*scroll_forward_fp)(volatile menu_choice_t*), void (*scroll_backward_fp)(volatile menu_choice_t *),
		void (*confirm_fp)(volatile menu_system_t *, volatile menu_choice_t *), void (*reject_fp)(volatile menu_system_t *, volatile menu_choice_t *));


extern volatile menu_system_t check_due_menu;




#endif /* MENU_CHECK_DUE_H_ */
