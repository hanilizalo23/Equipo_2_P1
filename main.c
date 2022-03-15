/**
	\file
	\brief P1, Multitask System
	\author Nelida Paulina Hernández Moya y Mauricio Peralta Osorio
	\date	13/03/2022
 */

#include "signal_generator.h"
#include "push_buttons.h"
#include "keypad.h"
#include "RGB.h"

#define SQUARE_PSWD 		123
#define DAC_PSWD 			789
#define MAXLENGTH 			  3
#define CONCATENATE_PSWD 	 10
#define PASSWORD COMPLETED   10

static uint8_t g_key = NO_KEY;

typedef enum {INIT, PSWD, SQUARE, DAC, CHANGE_DAC_SIGNAL, CHANGE_FREC_SIGNAL, WAIT} main_states_t; /**States as variables for determine the next case, not a machine state*/

int main(void) {

	static main_states_t state_main = INIT;
	uint8_t dac_on = FALSE;
	uint8_t square_on = FALSE;
    static uint32_t password = NOTHING;
	static uint8_t count = NOTHING;
	static uint8_t key = NO_KEY;

		while (1)
		{
			switch(state_main)
			{
			case INIT: /**Case for initialize everything*/
				signal_generator_init();
				push_buttons_init();
				keypad_init();
				rgb_init();
				PIT_stop(PIT_0);
				PIT_stop(PIT_2);
				state_main = PSWD;
			break;

			case PSWD: /**Case for entering the password for DAC and TOGGLE*/
				key = read_pressed_key();
				if((NO_KEY != key) && (MAXLENGTH > count))
				{
					password *= CONCATENATE_PSWD;
					password += key;
					count++;
					set_key(NO_KEY);
				}
				else if(MAXLENGTH == count)
				{
					if(SQUARE_PSWD == password)
					{
						state_main = SQUARE;
					}
					else if(DAC_PSWD == password)
					{
						state_main = DAC;
					}
					else
					{
						if(MAXLENGTH == count)
						{
							rgb_basic_off();
							red_on();
							count = NOTHING;
						}
						password = NOTHING;
						set_key(NO_KEY);
					}
				}
			break;

			case SQUARE: /**Case for the square signal*/
					if(MAXLENGTH == count)
					{
						rgb_basic_off();
						green_on();
						count = NOTHING;
					}
					key = read_pressed_key();
					if(KEY_STAR == key)
					{
						turn_square(DELAY_FOR_5HZ);
						count = NOTHING;
						password = NOTHING;
						set_key(NO_KEY);
					}
					else if(KEY_HASH == key)
					{
						PIT_stop(PIT_0);
						count = NOTHING;
						password = NOTHING;
						set_key(NO_KEY);
					}
					else if((KEY_ZERO <= key) && (KEY_NINE >= key))
					{
						count = NOTHING;
						password = NOTHING;
						state_main = PSWD;
					}
			break;


			case DAC: /**Case for the signals coming from DAC*/
				if(MAXLENGTH == count)
				{
					rgb_basic_off();
					green_on();
					count = NOTHING;
				}
				key = read_pressed_key();
				if(KEY_STAR == key)
				{
					start_sin();
					count = NOTHING;
					password = NOTHING;
					set_key(NO_KEY);
				}
				else if(KEY_HASH == key)
				{
					PIT_stop(PIT_2);
					count = NOTHING;
					password = NOTHING;
					set_key(NO_KEY);
				}
				else if((KEY_ZERO <= key) && (KEY_NINE >= key))
				{
					count = NOTHING;
					password = NOTHING;
					state_main = PSWD;
				}
			break;

			case CHANGE_DAC_SIGNAL: /**Handled by the correspondent API*/
			break;

			case CHANGE_FREC_SIGNAL: /**Handled by the correspondent API*/
			break;

			default:
				state_main = WAIT;
			break;
			}
		}
	}
