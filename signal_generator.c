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
#define SIN 	 &SM[3]
#define DEMO_DAC_BASEADDR 	   DAC0
#define FREQUENCIES 		   (3U)
#define TIME_DELAY       	   (0.001F)

/**State machine*/
typedef struct
{
	uint8_t out;
	const struct state_t* next[2];
}state_t;

/**Enum with the frequencies of the signals*/
typedef enum {freq5, freq10, freq15} base_time_t;

// Possible outputs
enum {state_none_out, state_sawtooth_out, state_triangle_out, state_sin_out};

// State machine
static const state_t SM[4]=
{
	{state_none_out, {NONE, SAWTOOTH}},
	{state_sawtooth_out, {SAWTOOTH, TRIANGLE}},
	{state_triangle_out, {TRIANGLE, SIN}},
	{state_sin_out, {SIN, SAWTOOTH}}
};

/**Configuration of output pins*/
gpio_pin_control_register_t pin_control_register_gpioc_pin3 = GPIO_MUX1;

void signal_generator_init (void)
{
	/**Configuration for the DAC [SDK])*/
    dac_config_t dacConfigStruct;
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(DEMO_DAC_BASEADDR, &dacConfigStruct);
    DAC_Enable(DEMO_DAC_BASEADDR, true);
    DAC_SetBufferReadPointer(DEMO_DAC_BASEADDR, 0U);

    /**Configuration for the square signal*/
	GPIO_clock_gating(GPIO_C);
	GPIO_pin_control_register(GPIO_C, bit_3, &pin_control_register_gpioc_pin3);
	GPIO_data_direction_pin(GPIO_C,GPIO_OUTPUT, bit_3);
	GPIO_clear_pin(GPIO_C, bit_3);

	/**NVIC configuration*/
   	NVIC_set_basepri_threshold(PRIORITY_10);
	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ,PRIORITY_6);
	NVIC_enable_interrupt_and_priotity(PIT_CH2_IRQ,PRIORITY_5);
	NVIC_enable_interrupt_and_priotity(PORTC_IRQ, PRIORITY_6);
	NVIC_global_enable_interrupts;

	/**PIT configuration*/
	PIT_clock_gating();
	PIT_enable();
	PIT_enable_interrupt(PIT_0);
	PIT_enable_interrupt(PIT_2);
}
