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

/**Configuration of output pins as known with passive filter  to avoid rebounds*/
gpio_pin_control_register_t pin_control_register_gpiob_keypad_rows_inputs = GPIO_MUX1 | GPIO_PFE | INTR_FALLING_EDGE;
gpio_pin_control_register_t pin_control_register_gpiod_keypad_columns_outputs = GPIO_MUX1;

static uint8_t g_key = NO_KEY;
static uint32_t g_col = COLUMN0;
static uint8_t g_key_pressed = FALSE;

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
	static uint8_t turn_off_led = 0;
	g_key_pressed = TRUE;
	g_col = next_column(g_col);
	GPIO_clear_pin(GPIO_D, g_col);

	/**For turning off the LED 1 second after it that was turned on*/
	if(GPIO_read_pin(GPIO_B, bit_22) || GPIO_read_pin(GPIO_E, bit_26))
	{
		turn_off_led++;
		if(LIMIT_TO_TURN_OFF_LED == turn_off_led)
		{
			GPIO_set_pin(GPIO_B, bit_22);
			GPIO_set_pin(GPIO_E, bit_26);
			turn_off_led = NOTHING;
		}
	}
}

uint32_t next_column(uint32_t col) /**This functions makes a sweep of the column to be on, after the one who was off*/
{
	switch(col)
	{
		case COLUMN0:
			GPIO_set_pin(GPIO_D, bit_0);
			return COLUMN1;
		break;

		case COLUMN1:
			GPIO_set_pin(GPIO_D, bit_1);
			return COLUMN2;
		break;

		case COLUMN2:
			GPIO_set_pin(GPIO_D, bit_2);
			return COLUMN3;
		break;

		case COLUMN3:
			GPIO_set_pin(GPIO_D, bit_3);
			return COLUMN0;
		break;

		default:
			return COLUMN0;
	}
	return FALSE;
}

uint8_t get_row(void) /**Saves the row in which was pressed a key*/
{
	static uint8_t row = NO_ROW;

	if(!GPIO_read_pin(GPIO_B, bit_2))
	{
		row = ROW_0;
		return row;
	}
	else if(!GPIO_read_pin(GPIO_B, bit_3))
	{
		row = ROW_1;
		return row;
	}
	else if(!GPIO_read_pin(GPIO_B, bit_10))
	{
		row = ROW_2;
		return row;
	}
	else if(!GPIO_read_pin(GPIO_B, bit_11))
	{
		row = ROW_3;
		return row;
	}
	else
	{
		row = NO_ROW;
	}

	return NO_ROW;
}

void get_pressed_key(void) /**Returns in the global variable the value of the pressed key, knowing the row and the column*/
{
	static uint8_t row = NO_ROW;
	row = get_row();

	if(TRUE == g_key_pressed)
	{
		switch(g_col)
		{
			case COLUMN0:
				if(ROW_0 == row)
				{
					g_key = KEY_ONE;
				}
				else if(ROW_1 == row)
				{
					g_key = KEY_FOUR;
				}
				else if(ROW_2 == row)
				{
					g_key = KEY_SEVEN;
				}
				else if(ROW_3 == row)
				{
					g_key = KEY_STAR;
				}
				else
				{
					g_key = NO_KEY;
				}
			break;

			case COLUMN1:
				if(ROW_0 == row)
				{
					g_key = KEY_TWO;
				}
				else if(ROW_1 == row)
				{
					g_key = KEY_FIVE;
				}
				else if(ROW_2 == row)
				{
					g_key = KEY_EIGHT;
				}
				else if(ROW_3 == row)
				{
					g_key = KEY_ZERO;
				}
				else
				{
					g_key = NO_KEY;
				}
			break;

			case COLUMN2:
				if(ROW_0 == row)
				{
					g_key = KEY_THREE;
				}
				else if(ROW_1 == row)
				{
					g_key = KEY_SIX;
				}
				else if(ROW_2 == row)
				{
					g_key = KEY_NINE;
				}
				else if(ROW_3 == row)
				{
					g_key = KEY_HASH;
				}
				else
				{
					g_key = NO_KEY;
				}
			break;

			case COLUMN3:
				if(ROW_0 == row)
				{
					g_key = KEY_A;
				}
				else if(ROW_1 == row)
				{
					g_key = KEY_B;
				}
				else if(ROW_2 == row)
				{
					g_key = KEY_C;
				}
				else if(ROW_3 == row)
				{
					g_key = KEY_D;
				}
				else
				{
					g_key = NO_KEY;
				}
			break;

			default:
				g_key = NO_KEY;
			break;
		}
	}
	g_key_pressed = FALSE;
}

uint8_t read_pressed_key(void) //*A getter for the pressed key.*/
{
	return g_key;
}

void set_key(uint8_t key) //*A setter for the key.*/
{
	g_key = key;
}
