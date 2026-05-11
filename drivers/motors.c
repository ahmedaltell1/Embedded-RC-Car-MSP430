/*
 * motors.c
 *
 *  Created on: Sep 29, 2024
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
extern char display_line[4][11];
extern volatile unsigned int ADC_Left_Detect;
extern volatile unsigned int ADC_Right_Detect;
extern volatile unsigned char update_display;
extern volatile unsigned int ADC_Thumb;
extern volatile unsigned char display_changed;
extern volatile unsigned int Which_Command_To_Do;

// Single wheel Forward and Reverse Functions
void R_Forward(void){
    P6OUT &= ~R_REVERSE;
    P6OUT |= R_FORWARD;
}

void L_Forward(void){
    P6OUT &= ~L_REVERSE;
    P6OUT |= L_FORWARD;
}

void R_Reverse(void){
    P6OUT &= ~R_FORWARD;
    P6OUT |= R_REVERSE;
}

void L_Reverse(void){
    P6OUT &= ~L_FORWARD;
    P6OUT |= L_REVERSE;
}

void SafetyCheck(void){
    if ((P6OUT & R_FORWARD) && (P6OUT & R_REVERSE)) {
        WheelsOff();
        P1OUT |= REDLED;
      }
    if ((P6OUT & L_FORWARD) && (P6OUT & L_REVERSE)) {
        WheelsOff();
        P1OUT |= REDLED;
     }
}



// Function to turn off all motors (call this function prior to reversing or moving forward)
void WheelsOff(void){

    RIGHT_FORWARD_SPEED = 0;
    RIGHT_REVERSE_SPEED = 0;
    LEFT_FORWARD_SPEED = 0;
    LEFT_REVERSE_SPEED = 0;

}

// Function to move wheels forward
void ForwardMovement(void){

    RIGHT_FORWARD_SPEED = 9000;             // make this one slower
    LEFT_FORWARD_SPEED = 15000;





}

// Function to move wheels backwards
void BackwardMovement(void){

    RIGHT_REVERSE_SPEED = 9000;
    LEFT_REVERSE_SPEED = 12500;
}





// function to keep moving until line is detected while on white pad
void Start_Moving_Function(void){
    RIGHT_FORWARD_SPEED = SLOW_R;             // make this one slower
    LEFT_FORWARD_SPEED = SLOW;

    if((ADC_Left_Detect >= 830) || (ADC_Right_Detect >= 830)){
            WheelsOff();
            Which_Command_To_Do = Line_Detected;
            CCR0_counter = 0;
            CCR1_counter = 0;

        }
}



void Turn_Onto_Circle_Function(void){

    if(CCR0_counter < 28){

        if((ADC_Right_Detect < 800)){
            RIGHT_FORWARD_SPEED = on_line_speed;
            LEFT_REVERSE_SPEED = on_line_speed;
            //CCR0_counter = 0;

        }

        else if(ADC_Left_Detect >= 800 || ADC_Right_Detect >= 800){         // change made here from OR to AND
            WheelsOff();
            //CCR0_counter = 0;
            CCR1_counter = 0;

        }
    }
    else{
        Which_Command_To_Do = Finish_The_Course;
        CCR0_counter = 0;
        CCR1_counter = 0;
        }


}

void Turn_Around_Circle_Function(void){

    if(CCR0_counter <= 180){


        if(ADC_Left_Detect >= 800 && ADC_Right_Detect >= 800){

            RIGHT_FORWARD_SPEED = 4500;
            LEFT_FORWARD_SPEED = 4500;

        }

        if(ADC_Left_Detect < 800 && ADC_Right_Detect > 800){

            RIGHT_FORWARD_SPEED = 0;
            LEFT_FORWARD_SPEED = 3000;
        }

        if(ADC_Right_Detect < 800 && ADC_Left_Detect > 800){

            RIGHT_FORWARD_SPEED = 3000;
            LEFT_FORWARD_SPEED = 0;


        }

    }

    else if(CCR0_counter > 180){
        WheelsOff();
        CCR0_counter = 0;
        CCR1_counter = 0;
        Which_Command_To_Do = OFF;
    }


}



void Turn_Onto_Line_Function(void){
// KEEP TURNING COUNTER CLOCKWISE IF LEFT DETECT OR RIGHT DETECT IS LESS THAN 800 ONCE 800 MOTOR OFF
        //CCR1_counter = 0;
if(CCR0_counter < 35){

    if((ADC_Left_Detect < 800)){
        LEFT_FORWARD_SPEED = on_line_speed;
        RIGHT_REVERSE_SPEED = on_line_speed;
        //CCR0_counter = 0;

    }

    else if(ADC_Left_Detect >= 800 || ADC_Right_Detect >= 800){         // change made here from OR to AND
        WheelsOff();
        //CCR0_counter = 0;
        CCR1_counter = 0;

    }
}
else{
    Which_Command_To_Do = Circle_and_Line;
    CCR0_counter = 0;
    CCR1_counter = 0;
    }


}

void Circle_Around_Line_Function(void){
    // 850-820 is the range usually so keep between 800-900
    // FOR THIS FUNCTION WE ARE ALREADY ON THE BLACK LINE, NOW WE WANT TO CIRCLE AROUND IT TWICE
    // TO DO THAT WE WANNA MAKE SURE BOTH DETECTORS ARE VERY CLOSE IN VALUE AT ALL TIMES
    // IF ONE DETECTORS VALUE IS SKEWED MORE THAN USUAL, THEN TURN THE WHEEL OF THE CORRESPONDING SIDE TO STAY ON TRACK

//if(CCR0_counter <= 230){


    if(ADC_Left_Detect >= 800 && ADC_Right_Detect >= 800){

        RIGHT_FORWARD_SPEED = 4000;
        LEFT_FORWARD_SPEED = 4000;

    }

    if(ADC_Left_Detect < 800 && ADC_Right_Detect > 800){

        RIGHT_FORWARD_SPEED = 0;
        LEFT_FORWARD_SPEED = 3000;
    }

    if(ADC_Right_Detect < 800 && ADC_Left_Detect > 800){

        RIGHT_FORWARD_SPEED = 3000;
        LEFT_FORWARD_SPEED = 0;


    }

    if(CCR0_counter >= 64){

        //Which_Command_To_Do = Turn_Onto_Circle;
        Which_Command_To_Do = Finish_The_Course;
        CCR0_counter = 0;
        CCR1_counter = 0;

    }


}



// Turn_Into_Circle_Function
// FOR THIS FUNCTION TURN THE LEFT WHEEL(SO RIGHT) UNTIL BOTH DETECTORS DETECT LESS THAN 800
// ONCE BOTH DETECT THAT THEN KEEP GOING STRAIGHT UNTIL U REACH MIDDLE, USING TIMERS SEE HOW LONG IT WILL TAKE TO REACH MIDDLE
// DONT FORGET TO DISPLAY TOTAL TIME TAKEN ON THE SCREEN TO COMPLETE CIRCLE


void Turn_Into_Circle_Function(void){

    // REVERSE FIRST TO HAVE HIGHER CHANCE GETTING RIGHT DETECTION

    // LEFT_REVERSE_SPEED = slow_circle_speed_skewed;
    // RIGHT_REVERSE_SPEED = slow_circle_speed;


    if(ADC_Left_Detect >= 730 || ADC_Right_Detect >= 730){      //

        LEFT_FORWARD_SPEED = slow_circle_speed;
        RIGHT_REVERSE_SPEED = slow_circle_speed;

    }

    if(ADC_Left_Detect < 730 && ADC_Right_Detect < 730){

        WheelsOff();
        CCR0_counter = 0;
        CCR1_counter = 0;
        Car_State = Travel_To_Middle;
    }


}


void Travel_To_Middle_Function(void){


    while(CCR0_counter <= 12){


        RIGHT_FORWARD_SPEED = SLOW_R;
        LEFT_FORWARD_SPEED = SLOW;

    }

    if(CCR0_counter > 12){

        WheelsOff();

    }


}

// After black line is detected and car turns onto it, this function handles entire line following and circling

void Travel_Around_Line_and_Cicle(void){

    if(CCR1_counter >= 31 && CCR1_counter <= 32){
        RIGHT_FORWARD_SPEED = 2500;
        LEFT_FORWARD_SPEED = 2500;
    }


    if(ADC_Left_Detect >= 800 && ADC_Right_Detect >= 800){

        RIGHT_FORWARD_SPEED = 3000;         // change to 3500
        LEFT_FORWARD_SPEED = 3000;

    }

    if(ADC_Left_Detect < 800 && ADC_Right_Detect > 800){

        RIGHT_FORWARD_SPEED = 0;
        LEFT_FORWARD_SPEED = 3000;
    }

    if(ADC_Right_Detect < 800 && ADC_Left_Detect > 800){

        RIGHT_FORWARD_SPEED = 3000;
        LEFT_FORWARD_SPEED = 0;


    }

    if(ADC_Left_Detect < 670 && ADC_Right_Detect < 670){

        //RIGHT_REVERSE_SPEED = 2500;
        //LEFT_REVERSE_SPEED = 2500;

        //RIGHT_FORWARD_SPEED = 0;
        //LEFT_FORWARD_SPEED = 0;
        LEFT_FORWARD_SPEED = 2200;
        RIGHT_FORWARD_SPEED = 1500;

    }


    if(CCR1_counter >= 120 && CCR1_counter <= 140){

        WheelsOff();

    }

}



//project 7 code below


/*
switch(Car_State){

      case Waiting:
          WheelsOff();
          strcpy(display_line[0], " WAITING  ");
          strcpy(display_line[1], "          ");
          strcpy(display_line[2], "RIGHT     ");
          strcpy(display_line[3], "LEFT      ");
          display_changed = TRUE;
          update_display = TRUE;

          if(CCR0_counter >= 30 && start == TRUE){

              Car_State = Start_Moving;
          }

          break;

      case IR_ON_OFF:
          if(IR_State == TRUE){
              strcpy(display_line[0], "    OFF   ");
              display_changed = TRUE;
              update_display = TRUE;
          }

          if(IR_State == FALSE){
              strcpy(display_line[0], "   ON     ");
              display_changed = TRUE;
              update_display = TRUE;
          }
          break;

      case Start_Moving:
          strcpy(display_line[0], "DETECTING ");
          display_changed = TRUE;
          update_display = TRUE;

          Start_Moving_Function();            //this function keeps moving until a black line is detected
          break;

      case Line_Detected:

          strcpy(display_line[0], "BLACK LINE");
          strcpy(display_line[1], "          ");
          strcpy(display_line[2], "RIGHT     ");
          strcpy(display_line[3], "LEFT      ");
          display_changed = TRUE;
          update_display = TRUE;

          CCR0_counter = 0;
          CCR1_counter = 0;

          Car_State = Turn_Onto_Line;
          break;

      case Turn_Onto_Line:

          strcpy(display_line[0], "TURN ONTO ");
          display_changed = TRUE;
          update_display = TRUE;


          Turn_Onto_Line_Function();          //this function turns onto the line after car stopped
          break;


      case Pause_Before_Circling:

          WheelsOff();

          break;

      case Circle_Around_Line:


          while(CCR1_counter < 30){
              WheelsOff();
          }

          if(CCR1_counter >= 30){
              strcpy(display_line[0], " CIRCLING ");
              display_changed = TRUE;
              update_display = TRUE;
              Circle_Around_Line_Function();
          }

          break;




     case Turn_Into_Circle:

         strcpy(display_line[0], " TURN  IN ");
         display_changed = TRUE;
         update_display = TRUE;

         Turn_Into_Circle_Function();
         break;


     case Travel_To_Middle:

         strcpy(display_line[0], " 1 MINUTE ");
         display_changed = TRUE;
         update_display = TRUE;

         Travel_To_Middle_Function();
         break;


      default: break;

      }


*/
