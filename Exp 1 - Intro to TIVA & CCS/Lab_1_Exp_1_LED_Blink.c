
/*
 * Name:        Lab_1_Exp_1_LED_Blink.c
 * Created:     17-01-2019
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


uint8_t led_color = 0x12;


int main(void)
{
    //Set CPU Clock to 20MHz. 400MHz PLL/2 = 200 DIV 10 = 20MHz
    SysCtlClockSet(SYSCTL_SYSDIV_10|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    while(1)
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, led_color);

        // since SysCtlDelay takes 3 instruction cycles to execute
        // 1 second delay = 1/(3*(1/F_CPU))
        SysCtlDelay(6670000);

        // to cycle the LED color through Red, Green and Blue
        led_color = (0x04 & led_color) ? 0x12 : (led_color >> 1);
    }
}
