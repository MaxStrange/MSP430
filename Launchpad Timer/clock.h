/*
 * clock.h
 *
 *  Created on: Jan 26, 2016
 *      Author: Max
 */

#ifndef CLOCK_H_
#define CLOCK_H_

void clock_init(void);
unsigned long clock_get_ms(void);
unsigned long clock_get_seconds(void);
void clock_start_timer(void);




#endif /* CLOCK_H_ */
