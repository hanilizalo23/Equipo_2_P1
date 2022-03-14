/*
* PIT.c
*
*  Created on: 19 feb 2022
*      Author: Mauricio Peralta Osorio
*/

#include <stdio.h>
#include <stdint.h>
#include "MK64F12.h"
#include "PIT.h"
#include "GPIO.h"
#include "Bits.h"

#define NOTHING 0U

static void (*PIT_0_callback)(void) = 0;
static void (*PIT_1_callback)(void) = 0;
static void (*PIT_2_callback)(void) = 0;
static void (*PIT_3_callback)(void) = 0;

pit_interrupt_flags_t pit_interrupt_flags = {0};

void PIT_delay(PIT_timer_t pit_timer, My_float_pit_t system_clock, My_float_pit_t delay) /**Delay for the PIT*/
{
	float LDVALUE = 0.0F;
	float clock_time = 0.0F;
	system_clock = system_clock / 2; /**Dividing the frequency of the K64 for the PIT*/
	clock_time = (1 / system_clock); /**Time is the inverse of the frequency*/
	LDVALUE = (delay / clock_time); /**Dividing the desired delay by the time of the clock*/
	LDVALUE = LDVALUE - 1; /**Subtracting one, because starts from 0*/

     switch(pit_timer) /** PIT to choose*/
           {
           case PIT_0:/** PIT 0 is selected*/
                PIT->CHANNEL[0].LDVAL = LDVALUE;
                PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
                break;
           case PIT_1:/** PIT 1 is selected*/
        	   	PIT->CHANNEL[1].LDVAL = LDVALUE;
        	   	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;
                break;
           case PIT_2:/** PIT 2 is selected*/
        	   	PIT->CHANNEL[2].LDVAL = LDVALUE;
        	   	PIT->CHANNEL[2].TCTRL |= PIT_TCTRL_TEN_MASK;
                break;
           case PIT_3:/** PIT 3 is selected*/
        	    PIT->CHANNEL[3].LDVAL = LDVALUE;
        	    PIT->CHANNEL[3].TCTRL |= PIT_TCTRL_TEN_MASK;
                break;
           default:/**If doesn't exist the option do nothing*/
           break;
           }
}

void PIT_clock_gating(void) /**Turn on the clock for the PIT*/
{
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
}

uint8_t PIT_get_interrupt_flag_status(PIT_timer_t pit) /**Status of the PIT flag*/
{
	uint8_t flag_status = 0;
	switch (pit)
	{
		case PIT_0:
			flag_status = pit_interrupt_flags.flag_PIT_CH0;
		break;

		case PIT_1:
			flag_status = pit_interrupt_flags.flag_PIT_CH1;
		break;

		case PIT_2:
			flag_status = pit_interrupt_flags.flag_PIT_CH2;
		break;

		case PIT_3:
			flag_status = pit_interrupt_flags.flag_PIT_CH3;
		break;

		default:
		break;
	}
	return flag_status;
}

void PIT_clear_interrupt_flag(PIT_timer_t pit) /**Clears the flag of the interrupt*/
{
	switch (pit)
	{
		case PIT_0:
			pit_interrupt_flags.flag_PIT_CH0 = 0;
		break;

		case PIT_1:
			pit_interrupt_flags.flag_PIT_CH1 = 0;
		break;

		case PIT_2:
			pit_interrupt_flags.flag_PIT_CH2 = 0;
		break;

		case PIT_3:
			pit_interrupt_flags.flag_PIT_CH3 = 0;
		break;

		default:
		break;
	}
}

void PIT_callback_init(PIT_timer_t pit, void(*handler)(void)) /**Callbacks for the PIT*/
{
	switch (pit)
	{
		case PIT_0:
			PIT_0_callback = handler;
		break;

		case PIT_1:
			PIT_1_callback = handler;
		break;

		case PIT_2:
			PIT_2_callback = handler;
		break;

		case PIT_3:
			PIT_3_callback = handler;
		break;

		default:
		break;
	}
}

void PIT_enable_interrupt(PIT_timer_t pit) /**Enabling the interrupt and starting the count*/
{
	switch (pit)
		{
			case PIT_0:
				PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TIE_MASK;
			break;

			case PIT_1:
				PIT->CHANNEL[1].TCTRL = PIT_TCTRL_TIE_MASK;
			break;

			case PIT_2:
				PIT->CHANNEL[2].TCTRL = PIT_TCTRL_TIE_MASK;
			break;

			case PIT_3:
				PIT->CHANNEL[3].TCTRL = PIT_TCTRL_TIE_MASK;
			break;

			default:
			break;
		}
}

void PIT_clear_interrupt(PIT_timer_t pit) /**Cleans the interruption of the PIT*/
{
	uint32_t dummyRead = NOTHING;

		switch (pit)
		{
			case PIT_0:
				PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;
				dummyRead = PIT->CHANNEL[0].TCTRL; /**Read control register for clear PIT flag, this is silicon bug*/
			break;

			case PIT_1:
				PIT->CHANNEL[1].TFLG |= PIT_TFLG_TIF_MASK;
				dummyRead = PIT->CHANNEL[1].TCTRL; /**Read control register for clear PIT flag, this is silicon bug*/
			break;

			case PIT_2:
				PIT->CHANNEL[2].TFLG |= PIT_TFLG_TIF_MASK;
				dummyRead = PIT->CHANNEL[2].TCTRL; /**Read control register for clear PIT flag, this is silicon bug*/
			break;

			case PIT_3:
				PIT->CHANNEL[3].TFLG |= PIT_TFLG_TIF_MASK;
				dummyRead = PIT->CHANNEL[3].TCTRL; /**Read control register for clear PIT flag, this is silicon bug*/
			break;

			default:
			break;
		}
}

void PIT_enable(void) /**Enables the controls and timer clocks for the PITs*/
{
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;
	PIT->MCR |= PIT_MCR_FRZ_MASK;
}

void PIT_start(PIT_timer_t pit_timer)  /**Starts the counting of the PIT*/
{
	switch (pit_timer)
	{
		case PIT_0:
			PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
		break;

		case PIT_1:
			PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;
		break;

		case PIT_2:
			PIT->CHANNEL[2].TCTRL |= PIT_TCTRL_TEN_MASK;
		break;

		case PIT_3:
			PIT->CHANNEL[3].TCTRL |= PIT_TCTRL_TEN_MASK;
		break;

		default:
		break;
	}
}

void PIT_stop(PIT_timer_t pit_timer) /**Stops the counting of the PIT*/
{
	switch (pit_timer)
	{
		case PIT_0:
			PIT->CHANNEL[0].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
		break;

		case PIT_1:
			PIT->CHANNEL[1].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
		break;

		case PIT_2:
			PIT->CHANNEL[2].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
		break;

		case PIT_3:
			PIT->CHANNEL[3].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
		break;

		default:
		break;
	}
}

/** ISRs of the PITs*/
void PIT0_IRQHandler(void)
{
	if(PIT_0_callback)
	{
		PIT_0_callback();
	}

	PIT_clear_interrupt_flag(PIT_0);
	PIT_clear_interrupt(PIT_0);
}

void PIT1_IRQHandler(void)
{
	if(PIT_1_callback)
	{
		PIT_1_callback();
	}

	PIT_clear_interrupt_flag(PIT_1);
	PIT_clear_interrupt(PIT_1);
}

void PIT2_IRQHandler(void)
{
	if(PIT_2_callback)
	{
		PIT_2_callback();
	}

	PIT_clear_interrupt_flag(PIT_2);
	PIT_clear_interrupt(PIT_2);
}

void PIT3_IRQHandler(void)
{
	if(PIT_3_callback)
	{
		PIT_3_callback();
	}

	PIT_clear_interrupt_flag(PIT_3);
	PIT_clear_interrupt(PIT_3);
}
