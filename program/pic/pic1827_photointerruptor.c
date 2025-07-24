/* 
* File:   newmain.c
* Author: oonishirikuhitoshi
*
* Created on October 13, 2023, 10:24 PM
*/

 

// PIC16F1827 Configuration Bit Settings

 

// 'C' source line config statements

 

// CONFIG1
#pragma config FOSC = ECH       // Oscillator Selection (ECH, External Clock, High Power Mode (4-32 MHz): device clock supplied to CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

 

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

 

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

 

#include <xc.h>

 

#include <stdio.h>
#include <stdlib.h>

 

unsigned char score = 0;

void init(){
    OSCCON = 0b01101010;    // 4MHz

    ANSELA = 0b00000000;
    TRISA = 0b00000010;
    PORTA = 0b00000000;

    ANSELB = 0b00000000;
    TRISB = 0b01111110;
    PORTB = 0b00000000;

    /* ?????Timer2??? */
    PIR1bits.TMR2IF = 0;
    TMR2 = 0;
    T2CON = 0b00000101;             // Timer2?On????????1/4
    PR2 = 10;                       // ??????????50???
    //PR2 = 255;
    PIE1 = 0b00000010;              // Timer2???????
    INTCON = 0b11000000;
            // GIE<7> ??????????
            // PEIE<6> ????????????
}

 

void i2c_slave_init(int address){
    SSP1MSK = 0b11111110;           // I2C?????<7:1>???????
    SSP1STAT = 0b10000000;         // I2C??????????(100kHz??1MHz)
    TRISB |= 0b00010010; //
    SSP1CON1 = 0b00110110;
            // SSPEN<5> RB4/RB1?SCL/SDA?????
            // SSPM<3:0> I2C????????7???????
    INTCON = 0b11000000;
            // GIE<7> ????????????
            // PEIE<6> ???????
    SSP1ADD = (address << 1) & 0xff;// I2C???????
    PIE1 |= 0b00001000;             // SSPIE1<3> MSSP1????????
    SSP1IF = 0;
    
}   



 

volatile int led_stat = 0;

 

void __interrupt() isr(void){
    if(PIR1bits.TMR2IF == 1){
        //led_stat = ~!led_stat;
        RA2 = !RA2;
        PIR1bits.TMR2IF = 0;
    }
    if(PIR1bits.SSP1IF == 1) {                 // I2C????????????????
        char buf = SSP1BUF;          // I2C???????????????
        if(SSP1STATbits.D_nA == 1){    // ?????????????
            if(SSP1STATbits.R_nW == 0){// ???????????
                
            }
        }

        if(SSP1STATbits.R_nW == 1){     //???????????
            if (SSP1STATbits.BF == 1){//address jyusinngo no warikomi
                buf = SSP1BUF;
                while(SSP1STATbits.BF);
                SSP1BUF = 0;
            }else{//date sousinngo no ACK uketori warikomi
                if(SSP1CON2bits.ACKSTAT == 0){
                    while(SSP1STATbits.BF);
                    SSP1BUF = score & 0b00001111;
                    score &= 0b11110000;
                    SSP1CON1bits.CKP = 1;
                }
            }
        }
        PIR1bits.SSP1IF = 0;           //???????????
        SSPCONbits.CKP = 1;           //??????????????
    }
}

 

/*
* score ... 0000**** 
*               ^^^^
*             RB6532  
*/


int main(int argc, char** argv) {
    // led ... RA0
    // ir sensor ... RA1
    // ir led ... RA2

    init();
    i2c_slave_init(8);

    // RA2 = 1;
    while(1){
        if(RB2 == 1){
            if((score ^ 0b00010000) & 0b00010000){ //flag = 1 dato score kasann sinai
                score |= 0b00010001;
            }
        }else{
            score &= 0b11101111;
        }
        if(RB3 == 1){
            if((score ^ 0b00100000) & 0b00100000){
                score |= 0b00100010;
            }
        }else{
            score &= 0b11011111;
        }
        if(RB5 == 1){
            if((score ^ 0b01000000) & 0b01000000){
                score |= 0b01000100;
            }
        }else{
            score &= 0b10111111;
        }
        if(RB6 == 1){
            if((score ^ 0b10000000) & 0b10000000){
                score |= 0b10001000;
            }
        }else{
            score &= 0b01111111;
        }
        
    }
    return (EXIT_SUCCESS);
}