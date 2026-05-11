/*
 * macros.h
 *
 *  Created on: Sep 12, 2024
 *      Author: ahmedaltell
 */

#ifndef MACROS_H_
#define MACROS_H_
#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define REDLED               (0x01) // RED LED 0
#define GRN_LED              (0x40) // GREEN LED 1
#define TEST_PROBE           (0x01) // 0 TEST PROBE
#define TRUE                 (0x01) //
#define P4PUD                (P4OUT)

// STATES ======================================================================
#define NONE ('N')
#define STRAIGHT ('L')
#define CIRCLE ('C')
#define EIGHT ('8')
#define TRIANGLE ('T')
#define WAIT ('W')
#define START ('S')
#define RUN ('R')
#define END ('E')
#define TURN ('A')
#define OFF (0)
#define ON (1)


#define WHEEL_COUNT_TIME (10)
#define RIGHT_COUNT_TIME (8)
#define LEFT_COUNT_TIME (7)
#define TRAVEL_DISTANCE (3)
#define WAITING2START (200)


// CIRCLE
#define CIRCLE_TRAVEL_DISTANCE (215) // keep between 200-250
#define CIRCLE_RIGHT_COUNT_TIME (1)
#define CIRCLE_LEFT_COUNT_TIME (9)
#define WHEEL_COUNT_CIRCLE (10)

#define CIRCLE_LEFT_RWHEEL (9)
#define CIRCLE_LEFT_LWHEEL (1)
// DONE


// EIGHT
#define EIGHT_TRAVEL_DISTANCE (115) // keep between 90-130
#define eight_right_count_time_bottom (9)
#define eight_left_count_time_bottom (0)
#define eight_right_count_time_top (0)
#define eight_left_count_time_top (9) // the motors are slightly different this should equal out


// TRIANGLE
// were taking left turns
#define TRIANGLE_TRAVEL_DISTANCE (10)
#define triangle_right_count_time (0)
#define triangle_left_count_time (13)
#define WHEEL_COUNT_TRIANGLE (23)



#define PRESSED (0)
#define RELEASED (1)
#define NOT_OKAY (0)
#define OKAY (1)
#define DEBOUNCE_TIME (8)
#define DEBOUNCE_RESTART (0)


// TIMERS
# define TB0CCR0_INTERVAL (50000)
# define TB0CCR1_INTERVAL (50000)
# define TB0CCR2_INTERVAL (50000)


// PWM
#define PWM_PERIOD (TB3CCR0)
#define LCD_BACKLITE_DIMING (TB3CCR1)
#define RIGHT_FORWARD_SPEED (TB3CCR2)
#define LEFT_FORWARD_SPEED (TB3CCR3)
#define RIGHT_REVERSE_SPEED (TB3CCR4)
#define LEFT_REVERSE_SPEED (TB3CCR5)

#define WHEEL_OFF (0)
#define SLOW_R (9000)
#define on_line_speed (5000)
#define skewed_speed (2000)

#define slow_circle_speed (3500)
#define slow_circle_speed_skewed (1200)

#define SLOW (15000)
#define FAST (50000)
#define PERCENT_100 (50000)
#define PERCENT_80 (45000)


// States for detection
#define Waiting (6)
#define Start_Moving (7)
#define Line_Detected (8)
#define Turn_Onto_Line (9)
#define IR_ON_OFF (10)
#define Circle_Around_Line (11)
#define Pause_Before_Circling (12)
#define Turn_Into_Circle (13)
#define Travel_To_Middle (14)
#define Turn_Onto_Circle (15)
#define Finish_The_Course (16)
#define Circle_and_Line (17)
#define black_2 (25)
#define leave_circle (30)
#define slow_straight (45)

// Buffers
#define BEGINNING (0)
#define RING_SIZE (128)

#define WAITING ('W')
#define RECEIVED ('R')
#define TRANSMIT ('T')



// SERIAL
#define forward (1)
#define reverse (2)
#define left_45 (3)
#define right_90 (4)
#define black_course (5)
#define straight_forward (20)


// Global Variables
volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
unsigned int okay_to_look_at_switch1;
unsigned int sw1_position;
unsigned int count_debounce_SW1;
unsigned int okay_to_look_at_switch2;
unsigned int sw2_position;

extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;

unsigned int moving;
unsigned int Last_Time_Sequence;                      // a variable to identify Time_Sequence has changed
unsigned int cycle_time;                              // is a new time base used to control making shapes
unsigned int time_change;                              // is an identifier that a change has occurred
unsigned int okay_to_look_at_switch1;
unsigned int sw1_position;
unsigned int count_debounce_SW1;
unsigned int count_debounce_SW2;
unsigned int okay_to_look_at_switch2;
unsigned int sw2_position;
volatile unsigned char IOT_2_PC[RING_SIZE];
volatile unsigned int iot_rx_wr;
unsigned int iot_rx_rd; // Only used in Main
unsigned int direct_iot; // Only used it Interrupt
volatile unsigned char PC_2_IOT[RING_SIZE];
volatile unsigned int usb_rx_wr;
unsigned int usb_rx_rd; // Only used in Main
unsigned int direct_usb; // Only used it Interrupt
extern volatile char not_command;


// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Carlson_StateMachine(void);


#endif /* MACROS_H_ */
