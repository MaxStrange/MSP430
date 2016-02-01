#ifndef CLOCK_H_
#define CLOCK_H_

void clock_init(void);
void clock_delay_ms(unsigned int to_delay);
unsigned long clock_get_ms(void);
unsigned long clock_get_seconds(void);
void clock_start_timer(void);




#endif /* CLOCK_H_ */
