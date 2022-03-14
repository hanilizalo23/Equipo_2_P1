/**
       \file
       \brief This is the source file for the colors needed in the main implementation.
       \author Nelida Paulina Hernández Moya
       \date 15/02/2022
*/

#include <stdio.h>
#include <stdint.h>
#include "MK64F12.h"
#include "GPIO.h"
#include "RGB.h"
#include "bits.h"

void rgb_init(void) /**Helps to do all the initialization of the GPIOs that corresponds to the RGB LED*/
{
	gpio_pin_control_register_t pcr_gpiob_pin_21 = GPIO_MUX1;
	gpio_pin_control_register_t pcr_gpiob_pin_22 = GPIO_MUX1;
	gpio_pin_control_register_t pcr_gpioe_pin_26 = GPIO_MUX1;

	GPIO_clock_gating(GPIO_B);
	GPIO_clock_gating(GPIO_E);

	GPIO_pin_control_register(GPIO_B, bit_21, &pcr_gpiob_pin_21);
	GPIO_pin_control_register(GPIO_B, bit_22, &pcr_gpiob_pin_22);
	GPIO_pin_control_register(GPIO_E, bit_26, &pcr_gpioe_pin_26);

	GPIO_set_pin(GPIO_B, bit_22);
	GPIO_set_pin(GPIO_B, bit_21);
	GPIO_set_pin(GPIO_E, bit_26);

	GPIO_data_direction_pin(GPIO_B, GPIO_OUTPUT, bit_21);
	GPIO_data_direction_pin(GPIO_B, GPIO_OUTPUT, bit_22);
	GPIO_data_direction_pin(GPIO_E, GPIO_OUTPUT, bit_26);
}

/**ON*/

void red_on(void) /**Turn on the red LED*/
{
	GPIO_clear_pin(GPIO_B, bit_22);
}

void blue_on(void) /**Turn on the blue LED*/
{
    GPIO_clear_pin(GPIO_B, bit_21);
}

void green_on(void) /**Turn on the green LED*/
{
	GPIO_clear_pin(GPIO_E, bit_26);
}

void purple_on(void) /**Turn on the purple LED*/
{
	GPIO_clear_pin(GPIO_B, bit_21);
	GPIO_clear_pin(GPIO_B, bit_22);
}

void yellow_on(void) /**Turn on the yellow LED*/
{
	GPIO_clear_pin(GPIO_B, bit_22);
	GPIO_clear_pin(GPIO_E, bit_26);
}

void white_on(void) /**Turn on the white LED*/
{
	GPIO_clear_pin(GPIO_B, bit_21);
	GPIO_clear_pin(GPIO_B, bit_22);
	GPIO_clear_pin(GPIO_E, bit_26);
}

/**OFF*/

void red_off(void) /**Turn off the red LED*/
{
	GPIO_set_pin(GPIO_B, bit_22);
}

void blue_off(void) /**Turn off the blue LED*/
{
    GPIO_set_pin(GPIO_B, bit_21);
}

void green_off(void) /**Turn off the green LED*/
{
	GPIO_set_pin(GPIO_E, bit_26);
}

void purple_off(void) /**Turn off the purple LED*/
{
	GPIO_set_pin(GPIO_B, bit_21);
	GPIO_set_pin(GPIO_B, bit_22);
}

void yellow_off(void) /**Turn off the yellow LED*/
{
	GPIO_set_pin(GPIO_B, bit_22);
	GPIO_set_pin(GPIO_E, bit_26);
}

void white_off(void) /**Turn off the white LED*/
{
	GPIO_set_pin(GPIO_B, bit_21);
	GPIO_set_pin(GPIO_B, bit_22);
	GPIO_set_pin(GPIO_E, bit_26);
}

void rgb_basic_off(void) /**Turn off the three colors of the LED*/
{
	green_off();
	blue_off();
	red_off();
}
