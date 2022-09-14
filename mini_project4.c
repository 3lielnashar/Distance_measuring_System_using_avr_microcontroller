/*
 * mini_project4.c
 *
 *  Created on: Oct 13, 2021
 *      Author: aliel
 */

#include"lcd.h"
#include"ultrasonic.h"

int main(void) {
	uint16 distance;
	LCD_init();
	Ultrasonic_init();
	LCD_displayString("distance :   cm");

	while (1) {

		distance = Ultrasonic_readDistance();
		LCD_moveCursor(0,10);
		if (distance >= 100) {
			LCD_intgerToString(distance);
		} else {
			LCD_intgerToString(distance);
			/* In case the digital value is three or two or one digits print space in the next digit place */
			LCD_displayCharacter(' ');
		}

	}

}
