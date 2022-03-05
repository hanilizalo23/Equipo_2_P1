/**
       \file
       \brief
                    This is the source file for the colors needed in the main implementation.
       \author Nelida Paulina Hernández Moya
       \date 15/02/2022
*/

#include <stdio.h>
#include <stdint.h>
#include "MK64F12.h"
#include "GPIO.h"
#include "RGB.h"
#include "bits.h"
#include "PIT.h"

typedef enum {seqmin, seqmax = 2}range_seq; /**Enum for the limits of the sequence*/
typedef enum {seqnw, seqsw2, seqsw3, seqsw23}number_seq; /**Enum for the values of the sequence*/
typedef enum {color0, color1, color2}number_color; /**Enum for the three colors of every sequence*/

/**Number of the pins for the reading*/
uint8_t sw2pin = 6;
uint8_t sw3pin = 4;

/**Variables for color and sequence*/
uint8_t color = 0;
uint8_t sequence = 0;

void SW3_cb(void) /**Setter for the callback of sw3 and both reading*/
{
	/**Starting the color of this sequence as the first*/
	color = color0;

	/**Variables for reading*/
	uint8_t sw2_read;
	uint8_t sw3_read;
	uint8_t sw_read;

	/**Reading the values of the switches*/
	sw2_read = GPIO_read_pin(GPIO_C, sw2pin);
	sw3_read = GPIO_read_pin(GPIO_A, sw3pin);

	/**An OR of the values*/
	sw_read = sw2_read | sw3_read;

	if (seqmin == sw_read) /**If the value is 0, it means, both swtiches are pressed, is the turn of the fourth sequence and cleaning the status*/
	{
		sequence = seqsw23;
		GPIO_clear_irq_status(GPIO_A);
		GPIO_clear_irq_status(GPIO_C);
	}
	else /**If the value is only for the switch 3, third sequence is going to run*/
	{
		sequence = seqsw3;
		GPIO_clear_irq_status(GPIO_A);
	}
}

void SW2_cb(void) /**Setter for the callback of sw2*/
{
	color = color0;
	sequence = seqsw2;
	GPIO_clear_irq_status(GPIO_C);
}

void PIT_cb(void) /**Callback for the PIT*/
{
	color++; /**Go switching colors*/

	if(seqmax < color) /**As long as the color sequence is behind the max limit, reset it*/
	{
		color = color0;
	}

	PIT_clear_interrupt_flag(); /**Cleaning the flag of PIT*/
}

/**Getters for the sequence and color*/

uint8_t get_sequence(void)
{
	return sequence;
}

uint8_t get_color(void)
{
	return color;
}

void green(void) /**Turn on the green LED, waiting and turning it off*/
{
	GPIO_clear_pin(GPIO_E, bit_26);
	GPIO_set_pin(GPIO_E, bit_26);
}

void blue(void) /**Turn on the blue LED, waiting and turning it off*/
{
    GPIO_clear_pin(GPIO_B, bit_21);
	GPIO_set_pin(GPIO_B, bit_21);
}

void purple(void) /**Turn on the purple LED, waiting and turning it off*/
{
	GPIO_clear_pin(GPIO_B, bit_21);
	GPIO_clear_pin(GPIO_B, bit_22);
	GPIO_set_pin(GPIO_B, bit_21);
	GPIO_set_pin(GPIO_B, bit_22);
}

void red(void) /**Turn on the red LED, waiting and turning it off*/
{
	GPIO_clear_pin(GPIO_B, bit_22);
	GPIO_set_pin(GPIO_B, bit_22);
}

void yellow(void) /**Turn on the yellow LED, waiting and turning it off*/
{
	GPIO_clear_pin(GPIO_B, bit_22);
	GPIO_clear_pin(GPIO_E, bit_26);
	GPIO_set_pin(GPIO_B, bit_22);
	GPIO_set_pin(GPIO_E, bit_26);
}

void white(void) /**Turn on the white LED, waiting and turning it off*/
{
	GPIO_clear_pin(GPIO_B, bit_21);
	GPIO_clear_pin(GPIO_B, bit_22);
	GPIO_clear_pin(GPIO_E, bit_26);
	GPIO_set_pin(GPIO_B, bit_21);
	GPIO_set_pin(GPIO_B, bit_22);
	GPIO_set_pin(GPIO_E, bit_26);
}

void no_color(void) /**Turn on none color*/
{
	GPIO_set_pin(GPIO_B, bit_21);
	GPIO_set_pin(GPIO_B, bit_22);
	GPIO_set_pin(GPIO_E, bit_26);
}
