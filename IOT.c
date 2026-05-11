/*
 * IOT.c
 *
 *  Created on: Oct 28, 2024
 *      Author: ahmedaltell
 *
 *  This file contains all handling of incoming remote control commands and give purpose to them
 *  Here the pad 8 black line course is handled
 */


#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"

extern volatile char command_buffer[6];
extern char display_line[4][11];

extern volatile char Fram_Command;
extern volatile unsigned int cmd_index;
extern volatile unsigned char IOT_2_PC[RING_SIZE];
volatile char not_command = 0;
extern char Serial_State;
extern volatile unsigned int CCR0_counter;
extern volatile unsigned int CCR1_counter;
extern volatile unsigned int Command_Sent;
volatile unsigned int reset_counter = 0;
volatile unsigned int Which_Command_To_Do = OFF;
extern volatile unsigned int ADC_Left_Detect;
extern volatile unsigned int ADC_Right_Detect;

void process_command(void){        // this function processes all inputs and checks whether command or regular comm


        switch(command_buffer[1]){

// pads 1-7 below
            case '1':
                strcpy(display_line[2], "ARRIVED 01");
                display_changed = TRUE;
                command_buffer[0] = 0;
                command_buffer[1] = 0;

                break;
            case '2':
                strcpy(display_line[2], "ARRIVED 02");
                display_changed = TRUE;
                command_buffer[0] = 0;
                command_buffer[1] = 0;
                break;
            case '3':
                strcpy(display_line[2], "ARRIVED 03");
                display_changed = TRUE;
                command_buffer[0] = 0;
                command_buffer[1] = 0;
                break;
            case '4':
                strcpy(display_line[2], "ARRIVED 04");
                display_changed = TRUE;
                command_buffer[0] = 0;
                command_buffer[1] = 0;
                break;
            case '5':
                strcpy(display_line[2], "ARRIVED 05");
                display_changed = TRUE;
                command_buffer[0] = 0;
                command_buffer[1] = 0;
                break;
            case '6':
                strcpy(display_line[2], "ARRIVED 06");
                display_changed = TRUE;
                command_buffer[0] = 0;
                command_buffer[1] = 0;
                break;
            case '7':
                strcpy(display_line[2], "ARRIVED 07");
                display_changed = TRUE;
                command_buffer[0] = 0;
                command_buffer[1] = 0;
                break;
// pad display done here

            case 'c':

                Which_Command_To_Do = slow_straight;
                CCR0_counter = 0;
                CCR1_counter = 0;
                command_buffer[0] = 0;
                command_buffer[1] = 0;

                break;

            case 'F':

                Which_Command_To_Do = straight_forward;
                CCR0_counter = 0;
                CCR1_counter = 0;
                command_buffer[0] = 0;
                command_buffer[1] = 0;

                break;

            case 'D':   // DRIVE forward

                Which_Command_To_Do = forward;
                CCR0_counter = 0;

                command_buffer[0] = 0;
                command_buffer[1] = 0;

                break;

            case 'B':   // REVERSE

                Which_Command_To_Do = reverse;
                CCR0_counter = 0;

                command_buffer[0] = 0;
                command_buffer[1] = 0;


                break;

            case 'L':   // LEFT turn 45 degrees

                Which_Command_To_Do = left_45;
                CCR0_counter = 0;

                command_buffer[0] = 0;
                command_buffer[1] = 0;


                break;

            case 'R':   // RIGHT turn 90 degrees

                Which_Command_To_Do = right_90;
                CCR0_counter = 0;

                command_buffer[0] = 0;
                command_buffer[1] = 0;


                break;


            case 'b':

                Which_Command_To_Do = black_course;
                //Which_Command_To_Do = Start_Moving;
                CCR0_counter = 0;
                //P2OUT ^= IR_LED;

                strcpy(display_line[2], "          ");
                strcpy(display_line[3], "          ");
                display_changed = TRUE;

                command_buffer[0] = 0;
                command_buffer[1] = 0;

                break;


            case 'O':

                Which_Command_To_Do = OFF;
                CCR0_counter = 0;

                command_buffer[0] = 0;
                command_buffer[1] = 0;

                break;

            case 'z':

                Which_Command_To_Do = black_2;
                CCR0_counter = 0;

                command_buffer[0] = 0;
                command_buffer[1] = 0;

                break;

            case 'l':

                Which_Command_To_Do = leave_circle;
                CCR0_counter = 0;

                command_buffer[0] = 0;
                command_buffer[1] = 0;

                break;


            default:
                break;


        }


}





void Phone_Commands(void){

    switch(Which_Command_To_Do){


    case OFF:
        WheelsOff();
    break;

    case slow_straight:

        if(CCR0_counter < 10){

        RIGHT_FORWARD_SPEED = 2500;
        LEFT_FORWARD_SPEED = 3000;

        }else {

            WheelsOff();

        }

        break;

    case forward:

       ForwardMovement();

       break;

    case straight_forward:

        if(CCR0_counter <= 10){
            RIGHT_FORWARD_SPEED = 15000;
            LEFT_FORWARD_SPEED = 20000;
        }

        if(CCR0_counter > 10 && CCR0_counter <= 14){

            LEFT_FORWARD_SPEED = 3000;
            RIGHT_FORWARD_SPEED = 0;

            if(CCR0_counter == 14){
                CCR0_counter = 0;
            }

        }


        break;


    case reverse:

        if(CCR0_counter < 15){
            BackwardMovement();
        }
        else{
            WheelsOff();
        }
    break;


    case left_45:

        if(CCR0_counter < 10){

        RIGHT_REVERSE_SPEED = 2500;
        LEFT_FORWARD_SPEED = 3000;

        }else {

            WheelsOff();

        }

        break;


    case right_90:

        if(CCR0_counter < 15){

        LEFT_REVERSE_SPEED = 3000;
        RIGHT_FORWARD_SPEED = 3000;

        }else {

            WheelsOff();

        }

        break;


    case black_course:  // move in a semi circle like manner until timer expires, try to land somewhere at the beginning or middle of pad

        strcpy(display_line[2], " BL START ");
        display_changed = TRUE;

        if(CCR0_counter >= 15){

            RIGHT_FORWARD_SPEED = 8000; // orig 8000
            LEFT_FORWARD_SPEED = 6000;

        }

        if(CCR0_counter >= 73){

            WheelsOff();
            CCR0_counter = 0;
            Which_Command_To_Do = Start_Moving;

        }


        break;

    case black_2:

        strcpy(display_line[2], " BL START ");
        display_changed = TRUE;

        if(CCR0_counter >= 15){

            RIGHT_FORWARD_SPEED = 7700;
            LEFT_FORWARD_SPEED = 6000;

        }

        if(CCR0_counter >= 60 && ADC_Left_Detect >= 850 || ADC_Right_Detect >= 850){

            WheelsOff();
            CCR0_counter = 0;
            Which_Command_To_Do = Start_Moving;

        }

        break;


    case Start_Moving:

        if(CCR0_counter >= 20){
          Start_Moving_Function();                  // after a small delay look for black line to intercept
        }


        break;

    case Line_Detected:

        strcpy(display_line[2], "INTERCEPT ");
        display_changed = TRUE;

        Which_Command_To_Do = Turn_Onto_Line;       // after detecting the line, call a function to turn onto it

        break;

    case Turn_Onto_Line:

        if(CCR0_counter >= 30){
            strcpy(display_line[2], " BL  TURN ");
            display_changed = TRUE;
            Turn_Onto_Line_Function();              //this function turns onto the line after car stopped for a period of time
        }

        break;


    case Circle_and_Line:   // in this state the car is looking to stay on the black line and start circling after the line is done

        if(CCR1_counter < 30){
            WheelsOff();
        }

        if(CCR1_counter >= 30){
            strcpy(display_line[2], "BL  TRAVEL");
            display_changed = TRUE;

            Travel_Around_Line_and_Cicle();
        }

        if(CCR1_counter >= 80){
            strcpy(display_line[2], "BL  CIRCLE");
            display_changed = TRUE;

        }

        if(CCR1_counter >= 310){

            WheelsOff();
            CCR0_counter = 0;
            CCR1_counter = 0;
            Which_Command_To_Do = OFF;

        }

        break;

    case leave_circle:

        WheelsOff();
        if(CCR0_counter >= 5 && CCR0_counter <= 35){
            strcpy(display_line[2], "BL  EXIT  ");
            display_changed = TRUE;
            ForwardMovement();
        }

        if(CCR0_counter > 35){
            strcpy(display_line[2], "BL  STOP  ");
            display_changed = TRUE;
            WheelsOff();
        }

        break;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





    case Circle_Around_Line:


        while(CCR1_counter < 30){
            WheelsOff();
        }

        if(CCR1_counter >= 30){
            display_changed = TRUE;
            update_display = TRUE;
            Circle_Around_Line_Function();
        }

        break;


    case Turn_Onto_Circle:

        while(CCR1_counter < 20){
            WheelsOff();
        }

        if(CCR1_counter >= 20){
            Turn_Onto_Circle_Function();
        }


        break;

    case Finish_The_Course:

        while(CCR1_counter < 10){
            WheelsOff();
        }

        if(CCR1_counter >= 10 && CCR1_counter <= 20){

            RIGHT_REVERSE_SPEED = 3000;
            LEFT_FORWARD_SPEED = 3000;

            if(ADC_Left_Detect >= 800 && ADC_Right_Detect >= 800){

                WheelsOff();

            }

        }

        if(CCR1_counter > 20 && CCR1_counter < 30){

            WheelsOff();

        }

        if(CCR1_counter >= 30){

            Turn_Around_Circle_Function();
        }

        break;



    default:
        break;
    }



}





