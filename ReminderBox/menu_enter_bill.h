#ifndef MENU_ENTER_BILL_H_
#define MENU_ENTER_BILL_H_


void menu_enter_bill_init(void (*scroll_forward_fp)(volatile menu_choice_t*), void (*scroll_backward_fp)(volatile menu_choice_t *),
		void (*confirm_fp)(volatile menu_system_t *, volatile menu_choice_t *), void (*reject_fp)(volatile menu_system_t *, volatile menu_choice_t *));


extern volatile menu_system_t enter_bill_menu;



#endif /* MENU_ENTER_BILL_H_ */
