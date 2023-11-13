#include "config.h"
#include <xc.h>
#include <stdint.h>
#define _XTAL_FREQ 4000000
//the following digits are the control of each 7 seg display
//they work active low because they are common cathod
#define digit1 RC0
#define digit2 RC1
#define digit3 RC2
#define digit4 RC3
#define digit5 RC4
#define digit6 RC5

uint8_t counter = 0; //this counter is to reach 1 second in TMR1
uint16_t preload = 0; //preload value that is preloaded to TMR1 (will be determined in main function)
unsigned int second = 0;//this is a counter for number of seconds
//it's max value will 235959 which is 24 hour clock
uint8_t h1,h2,m1,m2,s1,s2; //each one of those variables holds a digit

void timer1_timermode(uint8_t preloaded){//TMR1 initialization
    /*TMR1CS: Timer1 Clock Source Select bit
1 = External clock from pin RC0/T1OSO/T1CKI (on the rising edge)
0 = Internal clock (FOSC/4)*/
    TMR1CS = 0;
    /*T1CKPS1:T1CKPS0: Timer1 Input Clock Prescale Select bits
11 = 1:8 prescale value
10 = 1:4 prescale value
01 = 1:2 prescale value
00 = 1:1 prescale value*/
    T1CKPS0 = 0;
    T1CKPS1 = 0;
    
    TMR1 = preloaded; //preloaded value
    //interrupt enable
    TMR1IE = 1;
    TMR1IF = 0;//clear interrupt flag
    GIE = 1;
    PEIE = 1;
    /*TMR1ON: Timer1 On bit
1 = Enables Timer1
0 = Stops Timer1*/
    TMR1ON = 0;//initiallizing it with 0 first to make the stopwatch idle until i press the button to start it
}
void __interrupt()ISR(){
    if(TMR1IF){
        TMR1 = preload; //preloaded value to 
        counter++;//if this variable counts 20 times this means that 1 second passed
        if(counter == 20){ //this means that it counted 1 second
            second++; //this is the variable used to count how many seconds passed
            if(second == 999999) second = 0; //reset it because it's a 6 digit 7-segment = 999999 MAX VALUE
            counter = 0; //must reset the 1 second counter
        }
        TMR1IF = 0;//MUST clear the flag NEVER FORGET
    }
}
void main(void)  {
    //7-seg HEX code from 0~9 where Lsb doesn't exist(RB0)
    uint8_t hex_seg[] = {0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6};  //from 0 ~ 9  negelting the Lsb
    TRISB = 0x00; PORTB = 0x00; //outputs of 7-seg
    TRISC = 0x00; PORTC = 0x3F; //outputs of 7-seg digit control
    TRISD7 = 1; TRISD6 = 1; TRISD5 = 1; //inputs of button that control the stopwatch
    preload = 15536; //change this value to what ever you want to preload TMR1 with
    timer1_timermode(preload);
    while(1){
            if(RD5){//start/stop stopwatch
                __delay_ms(250);
                TMR1ON = ~TMR1ON;
                second = 0; counter = 0; preload = 15536;
            }
        h1 = second / 100000;
        h2 = ((second / 10000)%10);
        m1 = ((second / 1000)%10);
        m2 = ((second / 100)%10);
        s1 = ((second / 10)%10);
        s2 = second % 10;
        if(RD7){ //reset the stopwatch
            second = 0; counter = 0; preload = 15536;
        }
        if(RD6){ //pause/continue stopwatch
            __delay_ms(250);
            TMR1ON = ~TMR1ON;
        }
        PORTB = hex_seg[h1];
        digit1 = 0;
        __delay_us(150);
        digit1 = 1;
        PORTB = hex_seg[h2];
        digit2 = 0;
        __delay_us(150);
        digit2 = 1;
        PORTB = hex_seg[m1];
        digit3 = 0;
        __delay_us(150);
        digit3 = 1;
        PORTB = hex_seg[m2];
        digit4 = 0;
        __delay_us(150);
        digit4 = 1;
        PORTB = hex_seg[s1];
        digit5 = 0;
        __delay_us(150);
        digit5 = 1;
        PORTB = hex_seg[s2];
        digit6 = 0;
        __delay_us(150);
        digit6 = 1;
    }
    return;
}
