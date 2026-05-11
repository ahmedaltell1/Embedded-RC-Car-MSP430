/*
 * cases.c
 *
 *  Created on: Sep 28, 2024
 *      Author: ahmedaltell
 */

#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"


unsigned int time_change;
unsigned int cycle_time;
unsigned int right_motor_count;
unsigned int left_motor_count;
unsigned int segment_count;
unsigned int delay_start;
unsigned int circle_num = 0;
extern unsigned int event = NONE;
extern unsigned int state = NONE;
unsigned int Triangle;



void Run_Straight(void){


    switch(state){

    case WAIT:              //Begin
        wait_case();
    break;

    case START:             // Begin
        start_case();
        break;

    case RUN:               //Run
        run_case();
        break;

    case END:               //End
        end_case();
        break;

    default: break;
    }
}


void Run_Circle(void){

    switch(state){

    case WAIT:              //Begin
        wait_case();
        break;

    case START:             // Begin
        start_case();
        break;

    case RUN:               //Run
        run_circle_case();
        break;

    case END:               //End
        end_case();
        break;

    default: break;
    }

}

void Run_Eight(void){
    //state = WAIT;
    switch(state){

    case WAIT:              //Begin
        wait_case();
        break;

    case START:             // Begin
        start_case();
        break;

    case RUN:               //Run
        if(circle_num == 0){
            run_eight_case_bottom();
            break;
        } else{
            run_eight_case_top();
            break;
        }

    case END:               //End
        if (circle_num == 0) {
           state = WAIT;     // Go back to START for the next figure-8
           circle_num = 1;    // Reset for the next figure-8
        } else {
            end_case();        // After two figure-8s, stop the vehicle
            circle_num = 0;    // Reset the circle number
        }
          break;


    default: break;
    }
}

void Run_Triangle(void){
    //state = WAIT;
    switch(state){

    case WAIT:              //Begin
        wait_case();
        break;

    case START:             // Begin
        start_case();
        break;

    case RUN:               //Run
        if(Triangle%2 == 0){
            run_triangle_case();
        } else {
            run_case();
        }

        break;

    case END:               //End

        if(Triangle == 11){
            end_case();
        }else {
            Triangle++;
            state = WAIT;
        }
        break;
        default: break;
    }
}





// End of run cases


// Wait, Start, Run, End are defined below


void wait_case(void){
    if(time_change){
        time_change = 0;

    if(delay_start++ >= WAITING2START)
        {
        delay_start = 0;
        state = START;
        }
    }
}



void start_case(void){
    cycle_time = 0;
    right_motor_count = 0;
    left_motor_count = 0;
    ForwardMovement();
    segment_count = 0;
    state = RUN;
}



void run_case(void){
    if(time_change){
        time_change = 0;

    if(segment_count <= TRAVEL_DISTANCE){

        if(right_motor_count++ >= RIGHT_COUNT_TIME){
           P6OUT &= ~R_FORWARD;
            }

            if(left_motor_count++ >= LEFT_COUNT_TIME){
                P6OUT &= ~L_FORWARD;
            }

            if(cycle_time >= WHEEL_COUNT_TIME){
                cycle_time = 0;
                right_motor_count = 0;
                left_motor_count = 0;
                segment_count++;
                ForwardMovement();
               }

         }
            else{
                state = END;
            }
    }
}


void end_case(void){
    WheelsOff();
    state = WAIT;
    event = NONE;
    delay_start = 0;
}



//Functions to draw the SHAPES are all defined below


void run_circle_case(void){

     if(time_change){
         time_change = 0;

     // keep going until distance is completed
     if(segment_count <= CIRCLE_TRAVEL_DISTANCE){

         //right motor(slower)
         if(right_motor_count++ >= CIRCLE_RIGHT_COUNT_TIME){
             P6OUT &= ~R_FORWARD; // TURN OFF RIGHT WHEEL AFTER AMOUNT OF CYCLES
         }


         //left motor(faster wheel)
         if(left_motor_count++ >= CIRCLE_LEFT_COUNT_TIME){
             P6OUT &= ~L_FORWARD;
         }

         if(cycle_time >= WHEEL_COUNT_CIRCLE){
             cycle_time = 0;
             right_motor_count = 0;   // Reset right motor count
             left_motor_count = 0;    // Reset left motor count
             segment_count++;         // Increment segment count (progress in the circle)
             ForwardMovement();       // Move forward for the next segment
         }


      }else{
          state = END;

      }


   }

}




// EIGHT CASE STARTS HERE
void run_eight_case_top(void){

    if(time_change){
             time_change = 0;

         // keep going until distance is completed
         if(segment_count <= EIGHT_TRAVEL_DISTANCE){

             //right motor(slower)
             if(right_motor_count++ >= eight_right_count_time_top){
                 P6OUT &= ~R_FORWARD; // TURN OFF RIGHT WHEEL AFTER AMOUNT OF CYCLES
             }


             //left motor(faster wheel)
             if(left_motor_count++ >= eight_left_count_time_top){
                 P6OUT &= ~L_FORWARD;
             }

             if(cycle_time >= WHEEL_COUNT_CIRCLE){
                 cycle_time = 0;
                 right_motor_count = 0;   // Reset right motor count
                 left_motor_count = 0;    // Reset left motor count
                 segment_count++;         // Increment segment count (progress in the circle)
                 ForwardMovement();       // Move forward for the next segment
             }


          }else{
              state = END;

          }

    }
}


void run_eight_case_bottom(void){
    if(time_change){
             time_change = 0;

         // keep going until distance is completed
         if(segment_count <= EIGHT_TRAVEL_DISTANCE){

             //right motor(slower)
             if(right_motor_count++ >= eight_right_count_time_bottom){
                 P6OUT &= ~R_FORWARD; // TURN OFF RIGHT WHEEL AFTER AMOUNT OF CYCLES
             }


             //left motor(faster wheel)
             if(left_motor_count++ >= eight_left_count_time_bottom){
                 P6OUT &= ~L_FORWARD;
             }

             if(cycle_time >= WHEEL_COUNT_CIRCLE){
                 cycle_time = 0;
                 right_motor_count = 0;   // Reset right motor count
                 left_motor_count = 0;    // Reset left motor count
                 segment_count++;         // Increment segment count (progress in the circle)
                 ForwardMovement();       // Move forward for the next segment
             }


          }else{
              WheelsOff();              // IMPORTANT: for figure 8 after drawing bottom this turns off wheels then continues
              state = END;


          }


       }


}
// EIGHT CASE DONE



void run_triangle_case(void) {

    if(time_change){
           time_change = 0;

       if(segment_count <= TRIANGLE_TRAVEL_DISTANCE){

           if(right_motor_count++ >= triangle_right_count_time){
              P6OUT &= ~R_FORWARD;
               }

               if(left_motor_count++ >= triangle_left_count_time){
                   P6OUT &= ~L_FORWARD;
               }

               if(cycle_time >= WHEEL_COUNT_TRIANGLE){
                   cycle_time = 0;
                   right_motor_count = 0;
                   left_motor_count = 0;
                   segment_count++;
                   ForwardMovement();
                  }

            }
               else{
                   state = END;
               }
       }
}















