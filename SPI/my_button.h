#ifndef MY_BUTTON_H_
#define MY_BUTTON_H_

#define BUTTON BIT0
#define BUTTON_PRESSED ((P4IN & BUTTON) == 0x00)

void button_init(void);



#endif /* MY_BUTTON_H_ */
