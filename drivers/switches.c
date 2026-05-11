/*
 * switches.c
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

void switch_shape();
extern unsigned int event;
extern unsigned int state;
extern unsigned int delay_start;

unsigned int okay_to_look_at_switch1;
unsigned int sw1_position;
unsigned int count_debounce_SW1;
unsigned int count_debounce_SW2;
unsigned int okay_to_look_at_switch2;
unsigned int sw2_position;
unsigned int press_counter;
extern unsigned int Car_State;


void Switches_Process(void){
    Switch1_Process();
    Switch2_Process();
}


// Switch 1 Configurations
void Switch1_Process(void){

    if (okay_to_look_at_switch1 && sw1_position){
        if (!(P4IN & SW1)){
            sw1_position = PRESSED;
            okay_to_look_at_switch1 = NOT_OKAY;
            count_debounce_SW1 = DEBOUNCE_RESTART;
            press_counter++;

            }
        }


    if (count_debounce_SW1 <= DEBOUNCE_TIME){
        count_debounce_SW1++;
        }

    else{
        okay_to_look_at_switch1 = OKAY;
        if (P4IN & SW1){
            sw1_position = RELEASED;
        }
    }
}


// Switch 2 Configurations
void Switch2_Process(void){

    if (okay_to_look_at_switch2 && sw2_position){
        if (!(P2IN & SW2)){
            sw2_position = PRESSED;
            okay_to_look_at_switch2 = NOT_OKAY;
            count_debounce_SW2 = DEBOUNCE_RESTART;
            }
        }
    if (count_debounce_SW2 <= DEBOUNCE_TIME){
        count_debounce_SW2++;
    }
    else{
        okay_to_look_at_switch2 = OKAY;
        if (P2IN & SW2){
            sw2_position = RELEASED;
            }
    }
}
