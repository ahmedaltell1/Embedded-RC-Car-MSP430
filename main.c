//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//  Jim Carlson
//  Jan 2023
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"

// Function Prototypes
void main(void);

// Global Variables
volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned int CCR0_counter;
extern volatile unsigned int CCR1_counter;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
volatile unsigned int Time_Sequence;
volatile char one_time;
unsigned int test_value;
char chosen_direction;
char change;
unsigned int wheel_move;
char start_moving;
unsigned int moving;
unsigned int Last_Time_Sequence;                      // a variable to identify Time_Sequence has changed
unsigned int cycle_time;                              // is a new time base used to control making shapes
unsigned int time_change;                              // is an identifier that a change has occurred
extern unsigned int event;
extern unsigned int state;
extern unsigned int delay_start;
unsigned int Car_State = Waiting;
extern volatile unsigned int SW1_PRESSED;
extern volatile unsigned int SW2_PRESSED;

extern char iot_TX_buf[11];
extern char IOT_Ring_Rx[11];

extern volatile unsigned int start;
extern volatile unsigned int IR_State;

char Serial_State;

extern char TX_A0buf[RING_SIZE];
extern char RX_A0buf[RING_SIZE];

unsigned int x = 0;
extern volatile unsigned int Recieve_A0;

volatile char clear_buffer;
extern volatile char allow_transmit_to_PC;
extern volatile char Fram_Command;
extern volatile char command_buffer[6];

volatile unsigned int Order_Sent = 0;

extern volatile unsigned int first_time;

volatile char stop_save_command[] = "AT+SYSSTORE=0\r\n";
volatile char MUX_command[] = "AT+CIPMUX=1\r\n";
volatile char Port_command[] = "AT+CIPSERVER=1,6557\r\n";
volatile char response_buffer[64];
unsigned int response_index = 0;
volatile unsigned int Command_Sent = 0;

volatile unsigned int Command_Sent_Stop = 0;
extern volatile unsigned int reset_counter;

volatile unsigned int done_flag = 0;
volatile unsigned int reset_iot = 1;
extern volatile unsigned int start_processing;

void main(void){
  //  WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
  PM5CTL0 &= ~LOCKLPM5;
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings
  Init_Ports();                        // Initialize Ports
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_Timer_B0();                       // Initialize Timers
  Init_LCD();                          // Initialize LCD
  Init_ADC();
  Init_Timer_B3();
  Init_Serial_UCA0('a');                   // Initialize Serial Port for USB
  Init_Serial_UCA1('a');
  Reset_IOT();                          // RESET IOT SET LOW THEN BACK TO HIGH
  process_command();

  UCA0BRW = 4 ; // 115,200 baud, set to 17 for 460800
  UCA0MCTLW = 0x5551 ; // set to 0x4A00 for 460800

  UCA1BRW = 4 ; // 115,200 baud, set to 17 for 460800
  UCA1MCTLW = 0x5551 ; // set to 0x4A00 for 460800

// Place the contents of what you want on the display, in between the quotes
// Limited to 10 characters per line
  strcpy(display_line[0], "          ");
  strcpy(display_line[1], "          ");
  strcpy(display_line[2], "          ");
  strcpy(display_line[3], "          ");
  display_changed = TRUE;

//  Display_Update(0,0,0,0);

  wheel_move = 0;

//------------------------------------------------------------------------------
// Beginning of the "While" Operating System
//------------------------------------------------------------------------------

  WheelsOff();

  while(ALWAYS)
    {                                    // Can the Operating system run
      Display_Process();                 // Update Display
      P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF
      // Safety check, this function avoids forward and reverse to be set to high simultaneously
      SafetyCheck();

      if(start_processing == 1){
          process_command();
          start_processing = 0;
      }


     if (Command_Sent == 1 && CCR0_counter >= 50) {
          switch (Order_Sent) {
              case 0:
                  strcpy((char *)PC_2_IOT, stop_save_command); // copy command to TX buffer
                  usb_rx_wr = strlen(stop_save_command);
                  UCA0IE |= UCTXIE;                           // enable TX interrupt to send

                  // Wait for "OK" response
                  if (strstr((char *)IOT_2_PC, "OK")) {
                      memset((char *)IOT_2_PC, 0, sizeof(IOT_2_PC)); // clear RX buffer
                      Order_Sent = 1;                               // move to next command
                  }
                  break;

              case 1:
                  strcpy((char *)PC_2_IOT, MUX_command);
                  usb_rx_wr = strlen(MUX_command);      // set TX write index
                  UCA0IE |= UCTXIE;                     // enable TX interrupt to send

                  // Wait for "OK" response
                  if (strstr((char *)IOT_2_PC, "OK")) {
                      memset((char *)IOT_2_PC, 0, sizeof(IOT_2_PC)); // clear RX buffer
                      Order_Sent = 2;
                      display_changed = TRUE;
                  }
                  break;


              case 3:
                  if (!Command_Sent_Stop) {
                      strcpy((char *)PC_2_IOT, "AT+CIFSR\r\n");     // copy command to TX buffer
                      usb_rx_wr = strlen("AT+CIFSR\r\n");           // set TX write index
                      UCA0IE |= UCTXIE;
                  }

                  if (strstr((char *)IOT_2_PC, "+CIFSR:STAIP")) {
                      char full_ip_address[16] = {0};               // buffer to store the full IP
                      int start_index = -1, i = 0, j = 0;

                      Command_Sent_Stop = 1;

                      // locate the start of the IP address
                      for (i = 0; IOT_2_PC[i] != '\0'; i++) {
                          if (IOT_2_PC[i] == '"' && start_index == -1) {
                              start_index = i + 1; // Start after the first quote
                              break;
                          }
                      }

                      // extract the full IP address if the start index was found
                      if (start_index != -1) {
                          for (i = start_index; IOT_2_PC[i] != '\0'; i++) {
                              if (IOT_2_PC[i] == '"') { // Stop at the closing quote
                                  break;
                              }
                              if (j < sizeof(full_ip_address) - 1) {
                                  full_ip_address[j++] = IOT_2_PC[i];
                              }
                          }
                          full_ip_address[j] = '\0'; // Null-terminate the extracted IP
                      }

                      // split the IP address across two lines
                      char first_line[11] = {0};
                      char second_line[11] = {0};

                      strncpy(first_line, full_ip_address, 10);
                      strncpy(second_line, full_ip_address + 10, 10);


                      strncpy(display_line[0], first_line, 10);  // Display first part
                      strncpy(display_line[1], second_line, 10); // Display second part
                      display_changed = TRUE;

                      memset((char *)IOT_2_PC, 0, sizeof(IOT_2_PC));
                      Command_Sent = 0;
                      Order_Sent = 4;


                  }
                  break;


              case 2:

                  strcpy((char *)PC_2_IOT, Port_command); // Copy command to TX buffer
                  usb_rx_wr = strlen(Port_command);      // Set TX write index
                  UCA0IE |= UCTXIE;                     // Enable TX interrupt to send

                  // Wait for "OK" response
                  if (strstr((char *)IOT_2_PC, "OK")) {
                      memset((char *)IOT_2_PC, 0, sizeof(IOT_2_PC)); // Clear RX buffer
                      Order_Sent = 3;                               // Mark all commands sent
                      display_changed = TRUE;
                      done_flag = 1;
                  }
                  break;


              default:
                  break;
          }

      }




      if(done_flag == 1){   // once done sending commands,

          Phone_Commands();
      }

      if(!first_time){
          strcpy(display_line[2], "  Waiting  ");
          strcpy(display_line[3], " for input ");
      }

      if(first_time == 1){
          strcpy(display_line[2], "           ");
          strcpy(display_line[3], "           ");

      }



    } // End of While loop

}

