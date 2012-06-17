/*
 * led.h
 *
 *  Created on: 2011-11-15
 *      Author: xiaoyang
 */

#ifndef LED_H_
#define LED_H_

#include "types.h"

// GPIO_1[8] => PORTG[0] => LEDG[0]
// GPIO_1[9] => PORTG[1] => LEDR[0]

#define LEDG0		0		/*PORTG 0*/
#define LEDR0		1		/*PORTG 1*/

void led_init();
void led_off(int8u_t led_no);
void led_on(int8u_t led_no);

#endif /* LED_H_ */
