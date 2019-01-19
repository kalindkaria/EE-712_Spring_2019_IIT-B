
/*
 * Name:        Lab_1_HW_1.c
 * Created:     19-01-2019
 * Modified:    19-01-2019
 * Course:      EE-712 (Embedded System Design), IIT Bombay
 */


#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
//#include "driverlib/timer.h"
//#include "driverlib/interrupt.h"
//#include "driverlib/adc.h"


// macros definition
#define LOCK_F      (*((volatile unsigned long *)0x40025520))
#define CR_F        (*((volatile unsigned long *)0x40025524))

#define SW_1        GPIO_PIN_4
#define SW_2        GPIO_PIN_0

#define Red_LED     GPIO_PIN_1
#define Blue_LED    GPIO_PIN_2
#define Green_LED   GPIO_PIN_3


// global variables definition
uint8_t sw1_prev_cnt = 1, sw1_curr_cnt = 1, sw2_prev_cnt = 0x12, sw2_curr_cnt = 0x12, led_color = 0;
uint32_t delay = (6670000/2);   // value to generate delay of about 0.5 second


// initialize all the pins used in this experiment
void hardware_init()
{
    // set CPU Clock to 20MHz. 400MHz PLL/2 = 200 DIV 10 = 20MHz
    SysCtlClockSet(SYSCTL_SYSDIV_10|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    // enabling peripherals for PORT F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // unlock the GPIOCR register
    LOCK_F  = 0x4C4F434B;
    CR_F    = GPIO_PIN_0;

    // set the desired pins as input and output
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, SW_2|SW_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, Red_LED|Blue_LED|Green_LED);

    // program GPIO pad configuration to have weak pull-up
    GPIOPadConfigSet(GPIO_PORTF_BASE, SW_2|SW_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}


// check if SW1 pressed or not
void check_sw1()
{
    // SW_1 is not pressed
    if(GPIOPinRead(GPIO_PORTF_BASE, SW_1) == 0x10)
    {
        sw1_prev_cnt = sw1_curr_cnt;
    }
    // SW_1 is pressed
    else
    {
        if(sw1_prev_cnt == sw1_curr_cnt)
        {
            sw1_curr_cnt = (sw1_curr_cnt & 0x04) ? (0x01) : (sw1_curr_cnt << 1);
        }
    }
}


// check if SW2 pressed or not
void check_sw2()
{
    // SW_2 is not pressed
    if(GPIOPinRead(GPIO_PORTF_BASE, SW_2) == 0x01)
    {
        sw2_prev_cnt = sw2_curr_cnt;
    }
    // SW_2 is pressed
    else
    {
        if(sw2_prev_cnt == sw2_curr_cnt)
        {
            sw2_curr_cnt = (sw2_curr_cnt & 0x04) ? 0x12 : (sw2_curr_cnt >> 1);
        }
    }
}


int main(void)
{
    hardware_init();

    while(1)
    {
        check_sw1();    // to toggle delay of LED blink from 0.5 >> 1 >> 2

        check_sw2();    // to toggle color of LED from R >> G >> B

        led_color = (led_color > 0) ? 0 : sw2_curr_cnt;

        GPIOPinWrite(GPIO_PORTF_BASE, Red_LED|Blue_LED|Green_LED, led_color);
        SysCtlDelay(delay * sw1_curr_cnt);
    }
}
