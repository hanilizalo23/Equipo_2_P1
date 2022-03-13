/*
 * Created on: 03/12/2022
 * Author: Mauricio Peralta
 * @file    push_buttons.h
 */

#ifndef PUSH_BUTTONS_H_
#define PUSH_BUTTONS_H_

void push_buttons_init(void); /**Configuration for all the push buttons*/
void change_freq (void); /**When external buttons are pressed*/
void buttons_pressed(void); /**Default signal when system runs for the first time*/
void internal_sw_pressed(void); /**Changing the signals*/

#endif /* PUSH_BUTTONS_H_*/
