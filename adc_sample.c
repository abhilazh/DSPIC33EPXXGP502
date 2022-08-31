/*
 * File:   adc_sample.c
 * Author: Win10
 *
 * Created on July 29, 2022, 3:35 PM
 */


#define _XTAL_FREQ 12000000
//#define FOSC 12000000
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
#pragma config PLLKEN = ON              // PLL Lock Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// FOSC
#pragma config POSCMD = XT              // Primary Oscillator Mode Select bits (XT Crystal Oscillator Mode)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function bit (OSC2 is general purpose digital I/O pin)
#pragma config IOL1WAY = ON             // Peripheral pin select configuration (Allow only one reconfiguration)
#pragma config FCKSM = CSDCMD           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are disabled)

// FOSCSEL
#pragma config FNOSC = PRI              // Oscillator Source Selection (Primary Oscillator (XT, HS, EC))
#pragma config IESO = ON                // Two-speed Oscillator Start-up Enable bit (Start up device with FRC, then switch to user-selected oscillator source)

// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GCP = OFF                // General Segment Code-Protect bit (General Segment Code protect is Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.



#include "xc.h"
#include<math.h>
int resolution=1024;
 uint16_t adc_reading;
float reference_voltage=5,input_voltage=0;

void __delay_ms(unsigned int delay)
{
    for(int i=0;i<delay*1000;i++);
}

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

void UART1_txchara(unsigned char chara )
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

void ADC_Init()
{
    AD1CON1bits.ADON=0;//turn off adc
    AD1CON1bits.ADSIDL=0;//idle mode operation
    AD1CON1bits.AD12B=0;//10 bit config
    AD1CON1bits.FORM=0x00;//Data output format integer
    AD1CON1bits.ASAM=0;//sampling begin when the SAMP bit set
    AD1CON1bits.SAMP=0;//sample enable bit, sample and amplifiers are holding
    AD1CON1bits.SSRC=0x000;//manual trigger
    
    AD1CON2bits.VCFG=0x00;//reference voltage configuration bits
    AD1CON2bits.CSCNA=1;// scan input
    AD1CON2bits.CHPS=0x00;// 1 channel mode
    
    AD1CON3bits.ADRC=0;//Clock derived from system clock
    AD1CON3bits.SAMC=0x00001;//Auto sample time bits
    AD1CON3bits.ADCS=0x0;//adc1 conversion clock select bits
    
    AD1CON4bits.ADDMAEN=0;//Conversion results are stored in ADC1BUF0 through ADC1BUFF registers
    
    AD1CHS0bits.CH0NA=0; //Channel 0 Negative Input Select for Sample MUXA bit
    AD1CHS0bits.CH0SA=00000;//Channel 0 positive input is AN0(1,3)
   
}

void ADC_Enable()
{ 
    
    AD1CON1bits.ADON=1;//adc on
   
}

void ADC_Disable()
{
    AD1CON1bits.ADON=0;//adc off
}

void ADC_START_CONV()
{

    AD1CON1bits.SAMP = 1;
    __delay_ms(1000);
    AD1CON1bits.SAMP=0;
           
    while(!AD1CON1bits.DONE);
  
    adc_reading=ADC1BUF0;
    
    
    input_voltage=(adc_reading*(reference_voltage/1024));
   
}

int main(void)

{
    UART1_PinConfig();
    UART1_init();
    
    ANSELAbits.ANSA0=1;
    TRISAbits.TRISA0=1;
    ADC_Init();
    ADC_Enable();
  
    while(1)
    {
       
       ADC_START_CONV(); 
       __delay_ms(1000);
     
    }
}