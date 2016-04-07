#ifndef LED_H_
#define LED_H_

#define LED_MAX		(1 << 1)
#define LED_MISH	(1 << 2)

void led_init(void);
void led_blink(void);
void led_off(void);
void led_on(void);
void led_on_max(void);
void led_on_mish(void);
void led_off_max(void);
void led_off_mish(void);

#endif /* LED_H_ */
