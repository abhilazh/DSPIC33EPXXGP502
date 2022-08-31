/*
 * File:   LED_BLINK.c
 * Author: Win10
 *
 * Created on July 22, 2022, 3:58 PM
 */


#include "xc.h"

int main(void)
{
    TRISB=0X0000;
    
    while(1)
    {
        PORTBbits.RB10=1;
        //__delay_ms(1000);
        PORTBbits.RB10;
        
    }
    return 0;
}
