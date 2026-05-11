/*
 * ports.c
 *
 *  Created on: Sep 12, 2024
 *      Author: ahmedaltell
 */
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"

void Init_Ports(void);
void Init_Port1(void);
void Init_Port2(void);
void Init_Port3(void);
void Init_Port4(void);
void Init_Port5(void);
void Init_Port6(void);



void Init_Ports(void) {

    Init_Port1();
    Init_Port2();
    Init_Port3();
    Init_Port4();
    Init_Port5();
    Init_Port6();
}

// Configure PORT 1
void Init_Port1(void){
    P1OUT = 0x00;
    P1DIR = 0x00;

    P1SEL0 &= ~REDLED;      // GP/IO
    P1SEL1 &= ~REDLED;      // GP/IO
    P1OUT |= REDLED;       // Initial value = Low / Off
    P1DIR |= REDLED;        // Direction = output

    P1SELC |= V_A1_SEEED; // ADC input for A1_SEEED
    P1SELC |= V_DETECT_L; // ADC input for V_DETECT_L
    P1SELC |= V_DETECT_R; // ADC input for V_DETECT_R
    P1SELC |= V_A4_SEEED; // ADC input for V_A4_SEEED
    P1SELC |= V_THUMB; // ADC input for V_THUMB

    P1SEL0 |= UCA0RXD;     // GP/IO
    P1SEL1 &= ~UCA0RXD;     // GP/IO
    //P1OUT &= ~UCA0RXD;      //
    //P1DIR |= UCA0RXD;      // Direction = output

    P1SEL0 |= UCA0TXD;     // GP/IO
    P1SEL1 &= ~UCA0TXD;     // GP/IO
    //P1OUT &= ~UCA0TXD;      //
    //P1DIR |= UCA0TXD;      // Direction = output

}

// Configure PORT 2
void Init_Port2(void){

 P2OUT = 0x00;              // P2 set Low
 P2DIR = 0x00;              // Set P2 direction to output

 P2SEL0 &= ~SLOW_CLK;       // SLOW_CLK GPIO operation
 P2SEL1 &= ~SLOW_CLK;       // SLOW_CLK GPIO operation
 P2OUT &= ~SLOW_CLK;        // Initial Value = Low / Off
 P2DIR |= SLOW_CLK;         // Direction = output

 P2SEL0 &= ~CHECK_BAT;      // CHECK_BAT GPIO operation
 P2SEL1 &= ~CHECK_BAT;      // CHECK_BAT GPIO operation
 P2OUT &= ~CHECK_BAT;       // Initial Value = Low / Off
 P2DIR |= CHECK_BAT;        // Direction = output

 P2SEL0 &= ~IR_LED;         // P2_2 GPIO operation
 P2SEL1 &= ~IR_LED;         // P2_2 GPIO operation
 P2OUT &= ~IR_LED;          // Initial Value = Low / Off
 P2DIR |= IR_LED;           // Direction = input

 P2SEL0 &= ~SW2;            // SW2 Operation
 P2SEL1 &= ~SW2;            // SW2 Operation
 P2OUT |= SW2;              // Configure pullup resistor
 P2DIR &= ~SW2;             // Direction = input
 P2REN |= SW2;              // Enable pullup resistor
 P2IES |= SW2;              // P2.3 Hi/Lo edge interrupt
 P2IFG &= ~SW2;             // Clear all P2.3 interrupt flags
 P2IE |= SW2;               // P2.3 interrupt enabled

 P2SEL0 &= ~IOT_RUN_RED;    // IOT_RUN_CPU GPIO operation
 P2SEL1 &= ~IOT_RUN_RED;    // IOT_RUN_CPU GPIO operation
 P2OUT |= IOT_RUN_RED;     // Initial Value = Low / Off
 P2DIR |= IOT_RUN_RED;      // Direction = input

 P2SEL0 &= ~DAC_ENB;        // DAC_ENB GPIO operation
 P2SEL1 &= ~DAC_ENB;        // DAC_ENB GPIO operation
 P2OUT |= DAC_ENB;          // Initial Value = High
 P2DIR |= DAC_ENB;          // Direction = output

 P2SEL0 &= ~LFXOUT;         // LFXOUT Clock operation
 P2SEL1 |= LFXOUT;          // LFXOUT Clock operation

 P2SEL0 &= ~LFXIN;          // LFXIN Clock operation
 P2SEL1 |= LFXIN;           // LFXIN Clock operation

}



// Configure PORT 3
void Init_Port3(void){
    P3OUT = 0x00;
    P3DIR = 0x00;

    P3SEL0 &= ~TEST_PROBE;  // GP/IO
    P3SEL1 &= ~TEST_PROBE;  // GP/IO
    P3OUT &= ~TEST_PROBE;   // GP/IO
    P3DIR &= ~TEST_PROBE;   // Direction = output

    P3SEL0 &= ~OA2O;        // GP/IO
    P3SEL1 &= ~OA2O;        // GP/IO
    P3OUT &= ~OA2O;         //
    P3DIR &= ~OA2O;         // Direction = output

    P3SEL0 &= ~OA2N;        // GP/IO
    P3SEL1 &= ~OA2N;        // GP/IO
    P3OUT &= ~OA2N;         //
    P3DIR &= ~OA2N;         // Direction = output

    P3SEL0 &= ~OA2P;        // GP/IO
    P3SEL1 &= ~OA2P;        // GP/IO
    P3OUT &= ~OA2P;         //
    P3DIR &= ~OA2P;         // Direction = output

    P3SEL0 &= ~SMCLK_OUT;       // GP/IO
    P3SEL1 &= ~SMCLK_OUT;       // GP/IO
    P3OUT &= ~SMCLK_OUT;        //
    P3DIR &= ~SMCLK_OUT;        // Direction = output

    P3SEL0 &= ~DAC_CNTL;    // GP/IO
    P3SEL1 &= ~DAC_CNTL;    // GP/IO
    P3OUT &= ~DAC_CNTL;     //
    P3DIR &= ~DAC_CNTL;     // Direction = output

    P3SEL0 &= ~IOT_LINK_GRN;    // GP/IO
    P3SEL1 &= ~IOT_LINK_GRN;    // GP/IO
    P3OUT &= ~IOT_LINK_GRN;     //
    P3DIR &= ~IOT_LINK_GRN;     // Direction = output

    P3SEL0 &= ~IOT_EN;          // GP/IO
    P3SEL1 &= ~IOT_EN;          // GP/IO
    P3OUT &= ~IOT_EN;           //
    P3DIR |= IOT_EN;           // Direction = output

}


// Configure PORT 4
void Init_Port4(void){

 P4OUT = 0x00;              // P4 set Low
 P4DIR = 0x00;              // Set P4 direction to output

 P4SEL0 &= ~RESET_LCD;      // RESET_LCD GPIO operation
 P4SEL1 &= ~RESET_LCD;      // RESET_LCD GPIO operation
 P4OUT &= ~RESET_LCD;       // Initial Value = Low / Off
 P4DIR |= RESET_LCD;        // Direction = output

 P4SEL0 &= ~SW1;            // SW1 GPIO operation
 P4SEL1 &= ~SW1;            // SW1 GPIO operation
 P4PUD |= SW1;
 P4DIR &= ~SW1;             // Direction = input
 P4REN |= SW1;
 P4IES |= SW1;              // SW1 Hi/Lo edge interrupt
 P4IFG &= ~SW1;             // IFG SW1 cleared
 P4IE |= SW1;               // SW1 interrupt Enabled

 P4SEL0 |= UCA1TXD;         // USCI_A1 UART operation
 P4SEL1 &= ~UCA1TXD;        // USCI_A1 UART operation

 P4SEL0 |= UCA1RXD;         // USCI_A1 UART operation
 P4SEL1 &= ~UCA1RXD;        // USCI_A1 UART operation

 P4SEL0 &= ~UCB1_CS_LCD;    // UCB1_CS_LCD GPIO operation
 P4SEL1 &= ~UCB1_CS_LCD;    // UCB1_CS_LCD GPIO operation
 P4OUT |= UCB1_CS_LCD;      // Set SPI_CS_LCD Off [High]
 P4DIR |= UCB1_CS_LCD;      // Set SPI_CS_LCD direction to output

 P4SEL0 |= UCB1CLK;         // UCB1CLK SPI BUS operation
 P4SEL1 &= ~UCB1CLK;        // UCB1CLK SPI BUS operation

 P4SEL0 |= UCB1SIMO;        // UCB1SIMO SPI BUS operation
 P4SEL1 &= ~UCB1SIMO;       // UCB1SIMO SPI BUS operation

 P4SEL0 |= UCB1SOMI;        // UCB1SOMI SPI BUS operation
 P4SEL1 &= ~UCB1SOMI;       // UCB1SOMI SPI BUS operation

}



// Configure PORT 5
void Init_Port5(void){

    P5OUT = 0x00;
    P5DIR = 0x00;

    P5SEL0 &= ~V_BAT;       // GP/IO
    P5SEL1 &= ~V_BAT;       // GP/IO
    P5OUT &= ~V_BAT;        //
    P5DIR &= ~V_BAT;        // Direction = output

    P5SEL0 &= ~V_5_0;       // GP/IO
    P5SEL1 &= ~V_5_0;       // GP/IO
    P5OUT &= ~V_5_0;        //
    P5DIR &= ~V_5_0;        // Direction = output

    P5SEL0 &= ~V_DAC;       // GP/IO
    P5SEL1 &= ~V_DAC;       // GP/IO
    P5OUT &= ~V_DAC;        //
    P5DIR &= ~V_DAC;        // Direction = output

    P5SEL0 &= ~V_3_3;       // GP/IO
    P5SEL1 &= ~V_3_3;       // GP/IO
    P5OUT &= ~V_3_3;        //
    P5DIR &= ~V_3_3;        // Direction = output

    P5SEL0 &= ~IOT_BOOT;    // GP/IO
    P5SEL1 &= ~IOT_BOOT;    // GP/IO
    P5OUT |= IOT_BOOT;     //
    P5DIR |= IOT_BOOT;     // Direction = output

}



// Configure PORT 6
void Init_Port6(void){
    P6OUT = 0x00;
    P6DIR = 0x00;

    P6SEL0 &= ~LCD_BACKLITE;    // GP/IO
    P6SEL1 &= ~LCD_BACKLITE;    // GP/IO
    P6OUT |= LCD_BACKLITE;     // Initial Value = Low
    P6DIR |= LCD_BACKLITE;      // Direction = output

    P6SEL0 |= R_FORWARD;       // GP/IO
    P6SEL1 &= ~R_FORWARD;       // GP/IO
    //P6OUT &= ~R_FORWARD;        //
    P6DIR |= R_FORWARD;        // Direction = output

    P6SEL0 |= L_FORWARD;       // GP/IO
    P6SEL1 &= ~L_FORWARD;       // GP/IO
    //P6OUT &= ~L_FORWARD;        //
    P6DIR |= L_FORWARD;        // Direction = output

    P6SEL0 |= R_REVERSE;       // GP/IO
    P6SEL1 &= ~R_REVERSE;       // GP/IO
    //P6OUT &= ~R_REVERSE;        //
    P6DIR |= R_REVERSE;         // Direction = output

    P6SEL0 |= L_REVERSE;       // GP/IO
    P6SEL1 &= ~L_REVERSE;       // GP/IO
    //P6OUT &= ~L_REVERSE;         //
    P6DIR |= L_REVERSE;         // Direction = output

    P6SEL0 &= ~P6_5;            // GP/IO
    P6SEL1 &= ~P6_5;            // GP/IO
    P6OUT |= P6_5;             //
    P6DIR |= P6_5;             // Direction = output

    P6SEL0 &= ~GRN_LED;         // GP/IO
    P6SEL1 &= ~GRN_LED;         // GP/IO
    P6OUT |= GRN_LED;           // Initial Value = Low
    P6DIR |= GRN_LED;           // Direction = output
}
