 /******************************************************************************
 *
 * Module: ULTRASONIC
 *
 * File Name: ultrasonic.c
 *
 * Description: source file for the ultrasonic driver
 *
 * Author: Ali Elnashar
 *
 *******************************************************************************/
#include"gpio.h"
#include"ultrasonic.h"
#include <util/delay.h> /* For the delay functions */
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "icu.h"
#include"avr/io.h"
/*******************************************************************************
 *                      global_variables                                   *
 *******************************************************************************/
uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0;

/*******************************************************************************
 *                      Functions Definitions                                   *
 *******************************************************************************/

/*
 * Description :
 * initialize the ICU driver as required
 * Setup the ICU call back function
 * Setup the direction for the trigger pin as output pin through the GPIO driver..
 */
void Ultrasonic_init(void){
	/* Create configuration structure for ICU driver */
		Icu_ConfigType Icu_Config = {F_CPU_8,RISING};

		/* Enable Global Interrupt I-Bit */
		SREG |= (1<<7);
		/* Set the Call back function pointer in the ICU driver */
		Icu_setCallBack(Ultrasonic_edgeProcessing);
		Icu_init(&Icu_Config);
        GPIO_setupPinDirection(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN, PIN_OUTPUT);
        GPIO_writePin(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN, LOGIC_LOW);

}
/*
 * Description :
 *Send the Trigger pulse to the ultrasonic
 */
void Ultrasonic_Trigger(void){
	GPIO_writePin(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN, LOGIC_HIGH);
    _delay_us(10);
    GPIO_writePin(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN, LOGIC_LOW);
}
/*
 * Description :
 * Send the trigger pulse by using Ultrasonic_Trigger function.
 *Start the measurements by the ICU from this moment.
 */
uint16 Ultrasonic_readDistance(void){
	Ultrasonic_Trigger();

    return ((LIGHT_SPEED_IN_CM * g_timeHigh )/(2*CONVERTING_SECOND_TO_MICRO));

}
/*
 * Description :
 * This is the call back function called by the ICU driver.
 * This is used to calculate the high time (pulse time) generated by
 * the ultrasonic sensor.
 */
void Ultrasonic_edgeProcessing(void){
	g_edgeCount++;
		if(g_edgeCount == 1)
		{
			/*
			 * Clear the timer counter register to start measurements from the
			 * first detected rising edge
			 */
			Icu_clearTimerValue();
			/* Detect falling edge */
			Icu_setEdgeDetectionType(FALLING);
		}
		else if(g_edgeCount == 2)
		{
			/* Store the High time value */
			g_timeHigh = Icu_getInputCaptureValue();
			/* Detect rising edge */
			Icu_setEdgeDetectionType(RISING);
			g_edgeCount = 0;
		}


}
