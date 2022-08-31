


















































































/*
 * File:   uart_chara.c
 * Author: Win10
 *
 * Created on July 28, 2022, 11:39 AM
 */
//RESULT: WORKED PROPERLY GOT OUPUT AND VERIFIED

//#define FOSC=6000000
// DSPIC33EP32GP502 Configuration Bit Settings

// 'C' source line config statements

// FICD
#pragma config ICS = PGD2               // ICD Communication Channel Select bits (Communicate on PGEC2 and PGED2)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

// FPOR
#pragma config ALTI2C1 = OFF            // Alternate I2C1 pins (I2C1 mapped to SDA1/SCL1 pins)
#pragma config ALTI2C2 = OFF            // Alternate I2C2 pins (I2C2 mapped to SDA2/SCL2 pins)
#pragma config WDTWIN = WIN25           // Watchdog Window Select bits (WDT Window is 25% of WDT period)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler bits (1:32,768)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = OFF             // PLL Lock Enable bit (Clock switch will not wait for the PLL lock signal.)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// FOSC
#pragma config POSCMD = XT              // Primary Oscillator Mode Select bits (XT Crystal Oscillator Mode)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function bit (OSC2 is general purpose digital I/O pin)
#pragma config IOL1WAY = ON             // Peripheral pin select configuration (Allow only one reconfiguration)
#pragma config FCKSM = CSDCMD           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are disabled)

// FOSCSEL
#pragma config FNOSC = PRI              // Oscillator Source Selection (Primary Oscillator (XT, HS, EC))
#pragma config IESO = OFF               // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)

// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GCP = OFF                // General Segment Code-Protect bit (General Segment Code protect is Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

int count=0;
char REC_DATA;

void UART1_PinConfig()
{
       //Set the PPS
    
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS

    RPINR18bits.U1RXR = 0x002C;    //RB12->UART1:U1RX//RPI44
    RPOR4bits.RP43R = 0x0001;    //RB11->UART1:U1TX//RP43

    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
}

void UART1_init()
{
    U1MODE = 0x8008;
    U1STA = 0x400;
    U1BRG = 0x9B;
    U1MODEbits.UARTEN = 1;   // enabling UART ON bit
    U1STAbits.UTXEN = 1;
}

void UART1_txchara(unsigned char chara)
{
     while(U1STAbits.UTXBF == 1);
      U1STAbits.OERR = 0;
     IFS0bits.U1TXIF=0;
      U1TXREG = chara;
      
      
}

char UART1_rxchara()
{
   
    while(U1STAbits.URXDA==0);
    IFS0bits.U1RXIF=0;
    return U1RXREG;
        
}

void tx_string(unsigned char *string, unsigned int string_length)
{
    int i=0;
    for(i=0;i<string_length;i++)
    {
        UART1_txchara(string[i]);
    }
}

int main(void)

{
    char read_chara;
    UART1_PinConfig();
    UART1_init();
 
    while(1)
    {
          
        read_chara= UART1_rxchara();
       
 
        if(read_chara=='Y')
        {
            tx_string("Yes",3);
        }
        if(read_chara=='N')
        {
            tx_string("No",2);
        }
        
         if(U1STAbits.OERR==1)
        {
             U1STAbits.OERR = 0;
        }
         
    }
    return 0;
}
