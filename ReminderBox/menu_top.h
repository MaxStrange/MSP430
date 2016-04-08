#ifndef MENU_TOP_H_
#define MENU_TOP_H_

void scroll_top_menu_forward(volatile menu_choice_t *current_item, volatile menu_choice_t *current_sub_choice);
void scroll_top_menu_backward(volatile menu_choice_t *current_item, volatile menu_choice_t *current_sub_choice);
void confirm_top_menu(volatile menu_system_t *menu, volatile menu_choice_t *current_item);
void reject_top_menu(volatile menu_system_t *menu, volatile menu_choice_t *current_item);

extern volatile menu_system_t top_menu;

#endif /* MENU_TOP_H_ */
