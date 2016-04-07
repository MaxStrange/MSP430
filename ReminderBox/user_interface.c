#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "ui_model.h"

#include "user_interface.h"



void user_interface_init(void)
{
	ui_model_init();
}

void user_interface_display(void)
{
	ui_model_wake();
}
