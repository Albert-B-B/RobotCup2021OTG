
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>
#define _XTAL_FREQ 10000000

int antal = 0;
int steering = 1;

void __interrupt() ISR()         //Her lander programmet når det bliver afbrudt (interrupt))
{  
    if(TMR0IF==1)                //Hvis det er en TIMER0 interrupt
    {
        antal++;                 //Læg én til antal
        if(antal == 80)          //Er antal 1000?
        {
        antal = 0;               //Nulstil variablen antal
        }
        
        if (antal < 3+steering) {RA1 = 1;}
        else {RA1 = 0;}
        
        TMR0IF=0;                //Nulstil timer0 flaget så den kan interrupte igen
        TMR0 = 101;              //Indlæs ny værdi i TIMERregistret, (Note: Bemærk at den skal bruge én ekstra
                                                   //for at starte op igen, så 101 i stedet for 100  
        
    } 
}
void setup()                //Her foregår heleopsætningen af timerinterrupt
{
    OPTION_REGbits.PS2 = 0;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.T0CS = 0;
    TMR0=100;       // Load the time value for 1ms delay
    TMR0IE=1;       //Enable timer interrupt bit in PIE1 register
    GIE=1;          //Enable Global Interrupt
    PEIE=1;         //Enable the Peripheral Interrupt
    
    TRISA = 0b00000000;
}

void main()
{
    setup();
    while(1) {
        for (int i = 0; i<7; i++) {steering = i; __delay_ms(500);}
        for (int i = 6; i>0; i--) {__delay_ms(500); steering = i; }
    }
    
}