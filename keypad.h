/*
 * Created on: 03/12/2022
 * Author: Mauricio Peralta
 * @file    keypad.h
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <stdint.h>
#include "Bits.h"

#define GPIOB_OFF_CONST (0xFFFFFFFFU)
#define GPIOB_ON_CONST (0U)

#define COLUMN0 bit_0
#define COLUMN1 bit_1
#define COLUMN2 bit_2
#define COLUMN3 bit_3

#define ROW0	0x04
#define ROW1	0x08
#define ROW2	0x400
#define ROW3	0x800

typedef enum {ROW_0, ROW_1, ROW_2, ROW_3, NO_ROW} rows;

typedef enum {KEY_ZERO,
			  KEY_ONE,
			  KEY_TWO,
			  KEY_THREE,
			  KEY_FOUR,
			  KEY_FIVE,
			  KEY_SIX,
			  KEY_SEVEN,
			  KEY_EIGHT,
			  KEY_NINE,
			  KEY_A,
			  KEY_B,
			  KEY_C,
			  KEY_D,
			  KEY_HASH,
			  KEY_STAR,
			  NO_KEY,
}keys;

void keypad_init(void); /**Initialize the pins of the keypad*/
void change_column(void); /**Changing into columns*/
void get_pressed_key(void); /**Gets the pressed key*/
uint8_t read_pressed_key(void); /**Reads the pressed key*/
uint32_t next_column(uint32_t col); /**Gives the next column*/
uint8_t get_row(void); /**Gets the row*/
void set_key(uint8_t key); /**Setting the key of the keypad*/

#endif /* KEYPAD_H_ */
