/*
 * Created on: 03/12/2022
 * Author: Mauricio Peralta
 * @file    push buttons.c
 */

#include "push_buttons.h"
#include "GPIO.h"
#include "Bits.h"
#include "NVIC.h"
#include "PIT.h"
#include "signal_generator.h"

#define DELAY_FOR_SW (0.1F)
#define BOTH_PB	(0x040U)
#define B0		(0x840U)
#define B1		(0x440U)
#define SW2		(0xC00U)

/**Pin control configuration of GPIOS for the buttons with passive filter to avoid rebounds*/
gpio_pin_control_register_t pcr_gpioc_pin_10 = GPIO_MUX1 | GPIO_PE | GPIO_PS | GPIO_PFE | INTR_FALLING_EDGE;
gpio_pin_control_register_t pcr_gpioc_pin_11 = GPIO_MUX1 | GPIO_PE | GPIO_PS | GPIO_PFE | INTR_FALLING_EDGE;

/**Pin control configuration of GPIOS for the buttons of the K64 as pull ups*/
gpio_pin_control_register_t pcr_gpioa_pin_4 = GPIO_MUX1 | GPIO_PE | GPIO_PS | INTR_FALLING_EDGE;
gpio_pin_control_register_t pcr_gpioc_pin_6 = GPIO_MUX1 | GPIO_PE | GPIO_PS | INTR_FALLING_EDGE;

void push_buttons_init(void) /**Configuration for all the push buttons*/
{
	GPIO_clock_gating(GPIO_A);
	GPIO_clock_gating(GPIO_C);

	GPIO_pin_control_register(GPIO_A, bit_4, &pcr_gpioa_pin_4);
	GPIO_pin_control_register(GPIO_C, bit_6, &pcr_gpioc_pin_6);

	GPIO_pin_control_register(GPIO_C, bit_10, &pcr_gpioc_pin_10);
	GPIO_pin_control_register(GPIO_C, bit_11, &pcr_gpioc_pin_11);

	GPIO_data_direction_pin(GPIO_A, GPIO_INPUT, bit_4);
	GPIO_data_direction_pin(GPIO_C, GPIO_INPUT, bit_6);

	GPIO_data_direction_pin(GPIO_C, GPIO_INPUT, bit_10);
	GPIO_data_direction_pin(GPIO_C, GPIO_INPUT, bit_11);

	GPIO_callback_init(GPIO_A, internal_sw_pressed);
	GPIO_callback_init(GPIO_C, buttons_pressed);

	/**NVIC configuration*/
	NVIC_set_basepri_threshold(PRIORITY_10);
	NVIC_enable_interrupt_and_priotity(PORTA_IRQ, PRIORITY_4);
	NVIC_enable_interrupt_and_priotity(PORTC_IRQ, PRIORITY_2);
	NVIC_enable_interrupt_and_priotity(PIT_CH3_IRQ,PRIORITY_2);
	NVIC_global_enable_interrupts;

	/**PIT configuration*/
	PIT_clock_gating();
	PIT_enable();
	PIT_enable_interrupt(PIT_3);
	PIT_callback_init(PIT_3, change_freq);
}

void change_freq(void) /**When external buttons are pressed*/
{

}

void buttons_pressed(void) /**Default signal when system runs for the first time*/
{

}

void internal_sw_pressed(void) /**Changing the signals*/
{

}
