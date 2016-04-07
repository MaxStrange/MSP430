#ifndef UI_CONTROL_H_
#define UI_CONTROL_H_

#include "menu.h"

#define CONFIRM_BUTTON (1 << 4)
#define REJECT_BUTTON (1 << 3)
#define QEN_BLUE (1 << 6)
#define QEN_GREEN (1 << 7)



void ui_control_init(volatile bool *sleep_ptr, volatile menu_system_t *menu);
void ui_control_start(void);



#endif /* UI_CONTROL_H_ */
