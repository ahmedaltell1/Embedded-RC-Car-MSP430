/*
 * interrupts.c
 *
 *  Created on: Oct 4, 2024
 *      Author: ahmedaltell
 */
// done
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"

extern volatile unsigned char update_display;
extern volatile unsigned char display_changed;
volatile unsigned int CCR0_counter;
volatile unsigned int CCR1_counter;
extern volatile unsigned int SW1_PRESSED;
extern volatile unsigned int SW2_PRESSED;
extern volatile unsigned int count_presses;
extern volatile unsigned int reset_iot;



#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){

    // TimerB0 0 Interrupt handler
    //...... Add What you need happen in the interrupt ......
    CCR0_counter += 1;
    CCR1_counter += 1;
    update_display = TRUE;

    if(reset_iot == 2){
        P3OUT |= IOT_EN;
        reset_iot = 1;
    }

    //ADCCTL0 |= ADCSC;
    TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0

}


#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){

    // TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler

    switch(__even_in_range(TB0IV,14)){
    case 0: break;
        // No interrupt
    case 2: // CCR1 not used

        CCR1_counter += 1;

        switch (CCR1_counter) {
        case DEBOUNCE_TIME:                 // renables after 800ms
          if (SW1_PRESSED == PRESSED) {
            P4IFG &= ~SW1;                  // clear flags
            P4IE |= SW1;                    // enable switch interrupt
            //TB0CCTL1 &= ~CCIE;              // disable CCR1
            SW1_PRESSED = RELEASED;
          }

          if (SW2_PRESSED == PRESSED) {
            P2IFG &= ~SW2;                  // clear flags
            P2IE |= SW2;                    // enable switch interrupt
            TB0CCTL1 &= ~CCIE;              // disable CCR1
            SW2_PRESSED = RELEASED;

          }

        break;
        default: break;
        }
        TB0CCR1 += TB0CCR1_INTERVAL; // Add Offset to TBCCR1
        break;



    case 4: // CCR2 not used
        //...... Add What you need happen in the interrupt ......
        TB0CCR2 += TB0CCR2_INTERVAL; // Add Offset to TBCCR2
        break;
    case 14: // overflow
        //...... Add What you need happen in the interrupt ......
        break;
    default: break;
    }

}
