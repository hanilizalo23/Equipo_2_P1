/*
 * Created on: 03/12/2022
 * Author: Nelida Hernández
 * @file    signal_generator.c
 */

#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_dac.h"
#include "fsl_common.h"

#include "signal_generator.h"
#include "PIT.h"
#include "GPIO.h"
#include "Bits.h"
#include "NVIC.h"

/**States for the SM*/
#define NONE  	 &SM[0]
#define SAWTOOTH &SM[1]
#define TRIANGLE &SM[2]
#define SINUS 	 &SM[3]
#define DEMO_DAC_BASEADDR 	DAC0
#define DIF_FREQ 			(3U)
#define DELAY				(0.001F)

/**Structure for state machine*/
typedef const struct
{
	uint8_t out;
	const struct state_t* next[2];
}state_t;

/*Frequencies*/
typedef enum {freq5, freq10, freq15} base_time_t;

/**Outputs of the state machine*/
enum {state_none_out, state_sawtooth_out, state_triangle_out, state_sin_out};

// State machine
static const state_t SM[4]=
{
	{state_none_out, {NONE, SAWTOOTH}},
	{state_sawtooth_out, {SAWTOOTH, TRIANGLE}},
	{state_triangle_out, {TRIANGLE, SINUS}},
	{state_sin_out, {SINUS, SAWTOOTH}}
};

/**Variables for the state machine and the frequencies of every signal*/
static const state_t* current_state = NONE;
static uint8_t input = NOTHING;
static My_float_pit_t g_current_period = DELAY_FOR_15HZ;
static uint16_t *g_ptr_signals = sin_values;

void signal_generator_init (void)
{
	/**Configuration of the DAC*/
    dac_config_t dacConfigStruct;
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(DEMO_DAC_BASEADDR, &dacConfigStruct);
    DAC_Enable(DEMO_DAC_BASEADDR, true);
    DAC_SetBufferReadPointer(DEMO_DAC_BASEADDR, 0U);

    /**Configuration of output pin of the square signal*/
    gpio_pin_control_register_t pin_control_register_gpioc_pin3 = GPIO_MUX1;
	GPIO_clock_gating(GPIO_C);
	GPIO_pin_control_register(GPIO_C, bit_3, &pin_control_register_gpioc_pin3);
	GPIO_data_direction_pin(GPIO_C,GPIO_OUTPUT, bit_3);
	GPIO_clear_pin(GPIO_C, bit_3);

	/**Configuration of the NVIC*/
	NVIC_set_basepri_threshold(PRIORITY_10);
	NVIC_enable_interrupt_and_priotity(PIT_CH2_IRQ,PRIORITY_5);
	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ,PRIORITY_6);
	NVIC_enable_interrupt_and_priotity(PORTC_IRQ, PRIORITY_6);
	NVIC_global_enable_interrupts;

	/**Configuration of the PIT*/
	PIT_clock_gating();
	PIT_enable();
	PIT_enable_interrupt(PIT_0);
	PIT_callback_init(PIT_0, start_square);
	PIT_enable_interrupt(PIT_2);
	PIT_callback_init(PIT_2, dac_out);
	PIT_delay(PIT_2, SYSTEM_CLOCK, g_current_period);
}

void change(void)
{
	g_cycle_counter = NOTHING;
	input = TRUE;
	change_signal(current_state->out);
	input = FALSE;
}

void change_signal(signal signal)
{
	pause();
	switch(signal)
	{
		case no_signal:
			pause();
		break;

		case sawtooth:
			start_sawtooth();
		break;

		case sin:
			start_sin();
		break;

		case triangle:
			start_triangle();
		break;

		default:
		break;
	}
}

void turn_sin(void)
{
	g_ptr_signals = sin_values;
}

void turn_sawtooth(void)
{
	g_ptr_signals = sawtooth_values;
}


void turn_triangle(void)
{
	g_ptr_signals = triangle_values;
}

void send_square(My_float_pit_t modified_period)
{

}

void start_sin(void)
{
	turn_sin();
	PIT_delay(PIT_2, SYSTEM_CLOCK, g_current_period);
}

void start_sawtooth(void)
{
	turn_sawtooth();
	PIT_delay(PIT_2, SYSTEM_CLOCK, g_current_period);
}

void start_triangle(void)
{
	turn_triangle();
	PIT_delay(PIT_2, SYSTEM_CLOCK, g_current_period);
}

void start_square(void)
{

}

void pause(void)
{
	PIT_stop(PIT_2);
}

void signal_gen(void)
{
	change_signal(current_state->out);
}

void dac_out (void)
{

}

void change_period (My_float_pit_t period)
{

}
