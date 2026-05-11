/*
 * P4interrupts.c
 *
 *  Created on: Oct 4, 2024
 *      Author: ahmedaltell
 */


#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"

extern unsigned int Car_State;
extern volatile unsigned int CCR0_counter;
extern volatile unsigned int CCR1_counter;
volatile unsigned int SW1_PRESSED;
volatile unsigned int SW2_PRESSED;
volatile unsigned int count_presses;
volatile unsigned int IR_State = TRUE;
volatile int i;
volatile unsigned int start;
extern char Serial_State;
extern volatile unsigned int iot_rx_wr;

#pragma vector=PORT4_VECTOR
__interrupt void switchP4_interrupt(void){
    // Switch 1
    if (P4IFG & SW1) {
        P4IFG &= ~SW1;                // IFG SW1 cleared
        P4IE &= ~SW1;                 // interrupt disabled
        SW1_PRESSED = PRESSED;
        CCR0_counter = 0;
        CCR1_counter = 0;
        TB0CCTL0 |= CCIE;
        TB0CCTL1 |= CCIE;             // enable timer CCR1
        P2OUT ^= IR_LED;

        TB0CCTL1 &= ~CCIFG;                 //clear timerb0 interrupt flag
        TB0CCR1 = TB0R + TB0CCR1_INTERVAL;  // ADD Interval

    }
}

#pragma vector=PORT2_VECTOR
__interrupt void switchP2_interrupt(void){
    // Switch 2
    if (P2IFG & SW2) {
        SW2_PRESSED = PRESSED;
        CCR0_counter = 0;
        CCR1_counter = 0;
        P2IFG &= ~SW2;              // IFG SW2 cleared
        P2IE &= ~SW2;               // disable switch interrupt
        TB0CCTL1 |= CCIE;           // enable timer CCR1
        TB0CCTL0 |= CCIE;
        //P2OUT ^= IR_LED;

        TB0CCTL1 &= ~CCIFG;                 //clear timerb0 interrupt flag
        TB0CCR1 = TB0R + TB0CCR1_INTERVAL;  // ADD Interval


    }
}
