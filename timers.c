/*
 * timers.c
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


// Timer B0 initialization sets up both B0_0, B0_1-B0_2 and overflow


void Init_Timer_B0(void) {

    TB0CTL = TBSSEL__SMCLK;             // SMCLK source
    TB0CTL |= TBCLR;                    // Resets TB0R, clock divider, count direction
    TB0CTL |= MC__CONTINOUS;            // Continuous up
    TB0CTL |= ID__2;                    // Divide clock by 2
    TB0EX0 = TBIDEX__8;                 // Divide clock by an additional 8
    TB0CCR0 = TB0CCR0_INTERVAL;         // CCR0
    TB0CCTL0 |= CCIE;                   // CCR0 enable interrupt

    TB0CCR1 = TB0CCR1_INTERVAL;      // CCR1
    TB0CCTL1 |= CCIE;                // CCR1 enable interrupt
    TB0CCR2 = TB0CCR2_INTERVAL;      // CCR2
    TB0CCTL2 |= CCIE;                // CCR2 enable interrupt

    TB0CTL &= ~TBIE;                    // Disable Overflow Interrupt
    TB0CTL &= ~TBIFG;                   // Clear Overflow Interrupt flag

}

void Init_Timer_B3(void) {
//------------------------------------------------------------------------------
// SMCLK source, up count mode, PWM Right Side
// TB3.1 P6.0 LCD_BACKLITE
// TB3.2 P6.1 R_FORWARD
// TB3.3 P6.2 R_REVERSE
// TB3.4 P6.3 L_FORWARD
// TB3.5 P6.4 L_REVERSE
//------------------------------------------------------------------------------
    TB3CTL = TBSSEL__SMCLK;                     // SMCLK
    TB3CTL |= MC__UP;                           // Up Mode
    TB3CTL |= TBCLR;                            // Clear TAR
    PWM_PERIOD = WHEEL_PERIOD;                  // PWM Period [Set this to 50005]
    TB3CCTL1 = OUTMOD_7;                        // CCR1 reset/set
    LCD_BACKLITE_DIMING = PERCENT_80;           // P6.0 Right Forward PWM duty cycle
    TB3CCTL2 = OUTMOD_7;                        // CCR2 reset/set
    RIGHT_FORWARD_SPEED = WHEEL_OFF;            // P6.1 Right Forward PWM duty cycle
    TB3CCTL3 = OUTMOD_7;                        // CCR3 reset/set
    LEFT_FORWARD_SPEED = WHEEL_OFF;             // P6.2 Left Forward PWM duty cycle
    TB3CCTL4 = OUTMOD_7;                        // CCR4 reset/set
    RIGHT_REVERSE_SPEED = WHEEL_OFF;            // P6.3 Right Reverse PWM duty cycle
    TB3CCTL5 = OUTMOD_7;                        // CCR5 reset/set
    LEFT_REVERSE_SPEED = WHEEL_OFF;             // P6.4 Left Reverse PWM duty cycle
//------------------------------------------------------------------------------
}
