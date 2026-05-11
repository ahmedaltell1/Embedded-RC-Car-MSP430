/*
 * serial_interrupts.c
 *
 *  Created on: Oct 28, 2024
 *      Author: ahmedaltell
 */

#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"

extern char display_line[4][11];

extern volatile unsigned int SW1_PRESSED;
extern volatile unsigned int SW2_PRESSED;

char process_buffer[25];
extern char Serial_State;

volatile unsigned int temp_A1;
volatile unsigned int temp;


extern volatile unsigned int CCR0_counter;
extern volatile char clear_buffer;
extern volatile char allow_transmit_to_PC;
volatile char command_buffer[6] = {0};
volatile unsigned int index = 0;
volatile char Fram_Command = 0;
volatile unsigned int cmd_index = 0;
volatile unsigned int start_processing = 0;



volatile unsigned char IOT_2_PC[RING_SIZE];
volatile unsigned int iot_rx_wr;
unsigned int iot_rx_rd; // Only used in Main
unsigned int direct_iot; // Only used it Interrupt
volatile unsigned char PC_2_IOT[RING_SIZE];
volatile unsigned int usb_rx_wr;
unsigned int usb_rx_rd; // Only used in Main
unsigned int direct_usb; // Only used it Interrupt
extern volatile char not_command;
extern volatile unsigned int Command_Sent;
extern volatile unsigned int reset_iot;
volatile unsigned int first_time = 0;

#pragma vector=EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
//-----------------------------------------------------------------------------
// Echo back RXed character, confirm TX buffer is ready first
    switch(__even_in_range(UCA0IV,0x08)){
    static unsigned int command_index = 0;
        case 0:
            break; // Vector 0 - no interrupt

        case 2: // Vector 2 - RXIFG
            temp = iot_rx_wr++;
            cmd_index = temp;
            IOT_2_PC[temp] = UCA0RXBUF; // Rx -> IOT_2_PC character array
            if (iot_rx_wr >= (sizeof(IOT_2_PC))) {
                iot_rx_wr = BEGINNING; // circular buffer back to beginning
            }

            if (IOT_2_PC[temp] == '^') {
                first_time += 1;
                command_index = 0;
                command_buffer[command_index++] = '^'; // Add '^' to command buffer
                Fram_Command = 1;
            } else if (Fram_Command) {
                // continue capturing the command
                if (command_index < sizeof(command_buffer) - 1) {
                    command_buffer[command_index++] = IOT_2_PC[temp];
                }
                if (IOT_2_PC[temp] == '\n') {
                    // command is complete
                    command_buffer[command_index] = '\0'; // Null-terminate the command
                    Fram_Command = 0;                    // Reset command flag
                    //process_command();                    // Process the command
                    start_processing = 1;
                }
            }
            //strcpy(display_line[0], command_buffer);
            display_changed = TRUE;

            if (IOT_2_PC[temp] == '\n') {

                UCA1IE |= UCTXIE;
            }
            break;

        case 4: // Vector 4 - TXIFG
            UCA0TXBUF = PC_2_IOT[direct_iot++];
            if (direct_iot >= (sizeof(PC_2_IOT))){
                direct_iot = BEGINNING;
            }
            if (direct_iot == usb_rx_wr){
                UCA0IE &= ~UCTXIE; // Disable TX interrupt
            }
            break;

        default: break;
    }
//------------------------------------------------------------------------------
}


#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
// Echo back RXed character, confirm TX buffer is ready first
    switch(__even_in_range(UCA1IV,0x08)){
        case 0:
            break; // Vector 0 - no interrupt

        case 2: // Vector 2 - RXIFG
            temp = usb_rx_wr++;
            PC_2_IOT[temp] = UCA1RXBUF; // Rx -> PC_2_IOT character array
            if (usb_rx_wr >= (sizeof(PC_2_IOT))){
                usb_rx_wr = BEGINNING; // Circular buffer back to beginning
            }

            if ((PC_2_IOT[temp] == '\n')) {
              UCA0IE |= UCTXIE; // Enable Tx interrupt
            }

            break;

        case 4: // Vector 4 - TXIFG
            UCA1TXBUF = IOT_2_PC[direct_usb++];
            if (direct_usb >= (sizeof(IOT_2_PC))){
                direct_usb = BEGINNING; // Circular buffer back to beginning
            }
            if (direct_usb == iot_rx_wr){
                UCA1IE &= ~UCTXIE; // Disable TX interrupt
            }
            break;

        default: break;
    }
}



void Reset_IOT(void) {

    switch(reset_iot){

    case 1:

        P2OUT ^= IR_LED;

        reset_iot = 2;

        Command_Sent = 1;
        CCR0_counter = 0;

        break;

    default: break;

    }

}
