/*
 * Created on: 03/12/2022
 * Author: Mauricio Peralta
 * @file    keypad.c
 */

#include "Keypad.h"

#include "MK64F12.h"
#include "GPIO.h"
#include "NVIC.h"
#include "PIT.h"
#include "Bits.h"

#define DELAY_SWEEP 	(0.55F)
#define NOTHING			0
#define SYSTEM_CLOCK 	(21000000U)
#define LIMIT_TO_TURN_OFF_LED (19U)

/**Configuration of output pins as known with passive filter to avoid rebounds*/
gpio_pin_control_register_t pin_control_register_gpiob_keypad_rows_inputs = GPIO_MUX1 | GPIO_PFE | INTR_FALLING_EDGE;
gpio_pin_control_register_t pin_control_register_gpiod_keypad_columns_outputs = GPIO_MUX1;

static uint8_t g_key = NO_KEY;

void keypad_init(void)
{
	/**Configure of the pins as GPIOs of the rows and columns of the keypad*/
	GPIO_clock_gating(GPIO_B);
	GPIO_clock_gating(GPIO_D);

	GPIO_pin_control_register(GPIO_B, bit_2, &pin_control_register_gpiob_keypad_rows_inputs);
	GPIO_pin_control_register(GPIO_B, bit_3, &pin_control_register_gpiob_keypad_rows_inputs);
	GPIO_pin_control_register(GPIO_B, bit_10, &pin_control_register_gpiob_keypad_rows_inputs);
	GPIO_pin_control_register(GPIO_B, bit_11, &pin_control_register_gpiob_keypad_rows_inputs);

	GPIO_pin_control_register(GPIO_D, bit_0, &pin_control_register_gpiod_keypad_columns_outputs);
	GPIO_pin_control_register(GPIO_D, bit_1, &pin_control_register_gpiod_keypad_columns_outputs);
	GPIO_pin_control_register(GPIO_D, bit_2, &pin_control_register_gpiod_keypad_columns_outputs);
	GPIO_pin_control_register(GPIO_D, bit_3, &pin_control_register_gpiod_keypad_columns_outputs);

	GPIO_data_direction_pin(GPIO_B, GPIO_INPUT, bit_2);
	GPIO_data_direction_pin(GPIO_B, GPIO_INPUT, bit_3);
	GPIO_data_direction_pin(GPIO_B, GPIO_INPUT, bit_10);
	GPIO_data_direction_pin(GPIO_B, GPIO_INPUT, bit_11);

	GPIO_data_direction_pin(GPIO_D, GPIO_OUTPUT, bit_0);
	GPIO_data_direction_pin(GPIO_D, GPIO_OUTPUT, bit_1);
	GPIO_data_direction_pin(GPIO_D, GPIO_OUTPUT, bit_2);
	GPIO_data_direction_pin(GPIO_D, GPIO_OUTPUT, bit_3);

	GPIO_set_pin(GPIO_D, bit_0); //////////////
	GPIO_set_pin(GPIO_D, bit_1);
	GPIO_set_pin(GPIO_D, bit_2);
	GPIO_set_pin(GPIO_D, bit_3);

	PIT_clock_gating();
	PIT_enable();

	/**NVIC configuration*/
	NVIC_set_basepri_threshold(PRIORITY_10);
	NVIC_enable_interrupt_and_priotity(PIT_CH1_IRQ, PRIORITY_0);
	NVIC_enable_interrupt_and_priotity(PORTB_IRQ, PRIORITY_1);
	NVIC_global_enable_interrupts;

	/**PIT configuration*/
	PIT_enable_interrupt(PIT_1);
	g_key = NO_KEY;
	GPIO_callback_init(GPIO_B, get_pressed_key);
	PIT_callback_init(PIT_1, change_column);
	PIT_delay(PIT_1, SYSTEM_CLOCK, DELAY_SWEEP);
}

void change_column(void) /**This function change the column every 500 mS*/
{

}

uint32_t next_column(uint32_t col) /**This functions makes a sweep of the column to be on, after the one who was off*/
{

}

void get_pressed_key(void) /**Returns in the global variable the value of the pressed key, knowing the row and the column*/
{

}

uint8_t read_pressed_key(void) //*A getter for the pressed key*/
{
	return g_key;
}

void set_key(uint8_t key) //*A setter for the key*/
{
	g_key = key;
}
