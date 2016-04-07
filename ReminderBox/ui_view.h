#ifndef UI_VIEW_H_
#define UI_VIEW_H_

#include "menu.h"


void ui_view_init(volatile bool *sleep, volatile menu_system_t *menu_ptr);
void ui_view_display(void);



#endif /* UI_VIEW_H_ */
