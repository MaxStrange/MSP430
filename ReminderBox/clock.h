#ifndef CLOCK_H_
#define CLOCK_H_

#include <stdint.h>

void clock_init(void);
void clock_delay_ms(uint32_t to_delay);
uint32_t clock_get_ms(void);
uint32_t clock_get_seconds(void);
void clock_start_timer(void);




#endif /* CLOCK_H_ */
