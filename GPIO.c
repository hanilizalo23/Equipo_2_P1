/**
	\file
	\brief
		This is the source file for the GPIO device driver for Kinetis K64.
		It contains all the implementation for configuration functions and runtime functions.
		i.e., this is the application programming interface (API) for the GPIO peripheral.
	\author PhD. José Luis Pizano Escalante
	\completed by Mauricio Peralta Osorio
	\date	15/02/2022
	\todo
	    Interrupts are not implemented in this API implementation.
 */

#include <stdio.h>
#include <stdint.h>
#include "MK64F12.h"
#include "GPIO.h"
#include "bits.h"

static void (*gpio_C_callback)(void) = 0; /**Callback for the switch 2*/
static void (*gpio_A_callback)(void) = 0; /**Callback for the switch 3*/

static volatile gpio_interrupt_flags_t g_intr_status_flag = {0}; /**Status of the interruption flag, struct*/

uint8_t GPIO_clock_gating(gpio_port_name_t port_name)
{
	switch(port_name)/** Selecting the GPIO for clock enabling*/
		{
			case GPIO_A: /** GPIO A is selected*/
				SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTA; /** Bit 9 of SIM_SCGC5 is  set*/
				break;
			case GPIO_B: /** GPIO B is selected*/
				SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTB; /** Bit 10 of SIM_SCGC5 is set*/
				break;
			case GPIO_C: /** GPIO C is selected*/
				SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTC; /** Bit 11 of SIM_SCGC5 is set*/
				break;
			case GPIO_D: /** GPIO D is selected*/
				SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTD; /** Bit 12 of SIM_SCGC5 is set*/
				break;
			case GPIO_E: /** GPIO E is selected*/
				SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTE; /** Bit 13 of SIM_SCGC5 is set*/
				break;
			default: /**If doesn't exist the option*/
				return(FALSE);
		}// end switch
	/**Successful configuration*/
	return(TRUE);
}// end function

uint8_t GPIO_pin_control_register(gpio_port_name_t port_name, uint8_t pin,const gpio_pin_control_register_t*  pin_control_register)
{
	switch(port_name) /**Selecting the port and ping for the control register*/
		{
		case GPIO_A:/** GPIO A is selected*/
			PORTA->PCR[pin] = *pin_control_register;
			break;
		case GPIO_B:/** GPIO B is selected*/
			PORTB->PCR[pin] = *pin_control_register;
			break;
		case GPIO_C:/** GPIO C is selected*/
			PORTC->PCR[pin] = *pin_control_register;
			break;
		case GPIO_D:/** GPIO D is selected*/
			PORTD->PCR[pin] = *pin_control_register;
			break;
		case GPIO_E: /** GPIO E is selected*/
			PORTE->PCR[pin] = *pin_control_register;
		default:/**If doesn't exist the option*/
			return(FALSE);
		break;
		}
	/**Successful configuration*/
	return(TRUE);
}

void GPIO_write_port(gpio_port_name_t port_name, uint32_t data)
{
	switch(port_name) /**Writing in the full port a value*/
		{
		case GPIO_A:/** GPIO A is selected*/
			GPIOA->PDOR = data;
			break;
		case GPIO_B:/** GPIO B is selected*/
			GPIOB->PDOR = data;
			break;
		case GPIO_C:/** GPIO C is selected*/
			GPIOC->PDOR = data;
			break;
		case GPIO_D:/** GPIO D is selected*/
			GPIOD->PDOR = data;
			break;
		case GPIO_E: /** GPIO E is selected*/
			GPIOE->PDOR = data;
		default:/**If doesn't exist the option do nothing*/
		break;
		}
}

uint32_t GPIO_read_port(gpio_port_name_t port_name)
{
	switch(port_name) /**Reading and returning the value of a port*/
		{
		case GPIO_A:/** GPIO A is selected*/
			return GPIOA->PDIR;
			break;
		case GPIO_B:/** GPIO B is selected*/
			return GPIOB->PDIR;
			break;
		case GPIO_C:/** GPIO C is selected*/
			return GPIOC->PDIR;
			break;
		case GPIO_D:/** GPIO D is selected*/
			return GPIOD->PDIR;
			break;
		case GPIO_E: /** GPIO E is selected*/
			return GPIOE->PDIR;
		default:/**If doesn't exist the option do nothing*/
		break;
		}
	return 0;
}

uint8_t GPIO_read_pin(gpio_port_name_t port_name, uint8_t pin)
{
	switch(port_name) /**Reading and returning the value of a specific pin of the desired port*/
		{
		case GPIO_A:/** GPIO A is selected*/
			delay(100000); /**Shouldn't*/
			break;
		case GPIO_B:/** GPIO B is selected*/
			return (GPIOB->PDIR & 1<<pin);
			break;
		case GPIO_C:/** GPIO C is selected*/
			return (GPIOC->PDIR & 1<<pin);
			break;
		case GPIO_D:/** GPIO D is selected*/
			return (GPIOD->PDIR & 1<<pin);
			break;
		case GPIO_E: /** GPIO E is selected*/
			return (GPIOE->PDIR & 1<<pin);
		default:/**If doesn't exist the option do nothing*/
		break;
		}
	return 0;
}

void GPIO_set_pin(gpio_port_name_t port_name, uint8_t pin)
{
	switch(port_name) /**Setting the correspondent pin of a port*/
		{
		case GPIO_A:/** GPIO A is selected*/
			GPIOA->PSOR = 1<<pin;
			break;
		case GPIO_B:/** GPIO B is selected*/
			GPIOB->PSOR = 1<<pin;
			break;
		case GPIO_C:/** GPIO C is selected*/
			GPIOC->PSOR = 1<<pin;
			break;
		case GPIO_D:/** GPIO D is selected*/
			GPIOD->PSOR = 1<<pin;
			break;
		case GPIO_E: /** GPIO E is selected*/
			GPIOE->PSOR = 1<<pin;
		default:/**If doesn't exist the option do nothing*/
		break;
		}
}

void GPIO_clear_pin(gpio_port_name_t port_name, uint8_t pin)
{
	switch(port_name) /**Cleaning the correspondent pin of a port*/
		{
		case GPIO_A:/** GPIO A is selected*/
			GPIOA->PCOR = 1<<pin;
			break;
		case GPIO_B:/** GPIO B is selected*/
			GPIOB->PCOR = 1<<pin;
			break;
		case GPIO_C:/** GPIO C is selected*/
			GPIOC->PCOR = 1<<pin;
			break;
		case GPIO_D:/** GPIO D is selected*/
			GPIOD->PCOR = 1<<pin;
			break;
		case GPIO_E: /** GPIO E is selected*/
			GPIOE->PCOR = 1<<pin;
		default:/**If doesn't exist the option do nothing*/
		break;
		}
}

void GPIO_toogle_pin(gpio_port_name_t port_name, uint8_t pin)
{
	switch(port_name) /**Toggling the correspondent pin of a port*/
		{
		case GPIO_A:/** GPIO A is selected*/
			GPIOA->PTOR = 1<<pin;
			break;
		case GPIO_B:/** GPIO B is selected*/
			GPIOB->PTOR = 1<<pin;
			break;
		case GPIO_C:/** GPIO C is selected*/
			GPIOC->PTOR = 1<<pin;
			break;
		case GPIO_D:/** GPIO D is selected*/
			GPIOD->PTOR = 1<<pin;
			break;
		case GPIO_E: /** GPIO E is selected*/
			GPIOE->PTOR = 1<<pin;
		default:/**If doesn't exist the option do nothing*/
		break;
		}
}

void GPIO_data_direction_port(gpio_port_name_t port_name,uint32_t direction)
{
	switch(port_name) /**Configuring the direction of a port for input or output*/
		{
		case GPIO_A:/** GPIO A is selected*/
			GPIOA->PDDR |= direction;
			break;
		case GPIO_B:/** GPIO B is selected*/
			GPIOB->PDDR |= direction;
			break;
		case GPIO_C:/** GPIO C is selected*/
			GPIOC->PDDR |= direction;
			break;
		case GPIO_D:/** GPIO D is selected*/
			GPIOD->PDDR |= direction;
			break;
		case GPIO_E: /** GPIO E is selected*/
			GPIOE->PDDR |= direction;
		default:/**If doesn't exist the option do nothing*/
		break;
		}
}

void GPIO_data_direction_pin(gpio_port_name_t port_name, uint8_t state, uint8_t pin)
{
	switch(port_name) /**Configuring the pin of a port as an input or output*/
		{
		case GPIO_A:/** GPIO A is selected*/
			GPIOA->PDDR |= state<<pin;
			break;
		case GPIO_B:/** GPIO B is selected*/
			GPIOB->PDDR |= state<<pin;
			break;
		case GPIO_C:/** GPIO C is selected*/
			GPIOC->PDDR |= state<<pin;
			break;
		case GPIO_D:/** GPIO D is selected*/
			GPIOD->PDDR |= state<<pin;
			break;
		case GPIO_E: /** GPIO E is selected*/
			GPIOE->PDDR |= state<<pin;
		default:/**If doesn't exist the option do nothing*/
		break;
		}
}

void PORTC_IRQHandler(void) /**Verifies if the interrupt was from port C and calls the correspondent function*/
{
	if(gpio_C_callback)
	{
		gpio_C_callback();
	}
	GPIO_clear_interrupt(GPIO_C);

}

void PORTA_IRQHandler(void) /**Verifies if the interrupt was from port A and calls the correspondent function*/
{
	if(gpio_A_callback)
	{
		gpio_A_callback();
	}

	GPIO_clear_interrupt(GPIO_A);
}

void GPIO_clear_interrupt(gpio_port_name_t port_name)
{
	switch(port_name)/** Selecting the GPIO for cleaning*/
	{
		case GPIO_A: /** GPIO A is selected*/
			PORTA->ISFR=0xFFFFFFFF;
			break;
		case GPIO_B: /** GPIO B is selected*/
			PORTB->ISFR=0xFFFFFFFF;
			break;
		case GPIO_C: /** GPIO C is selected*/
			PORTC->ISFR = 0xFFFFFFFF;
			break;
		case GPIO_D: /** GPIO D is selected*/
			PORTD->ISFR=0xFFFFFFFF;
			break;
		default: /** GPIO E is selected*/
			PORTE->ISFR=0xFFFFFFFF;
			break;
	}// end switch
}

void GPIO_callback_init(gpio_port_name_t port_name,void (*handler)(void)) /**Assigns the function to execute according to the port*/
{
	if(GPIO_A == port_name)
	{
		gpio_A_callback = handler;
	}
	else
	{
		gpio_C_callback = handler;
	}
}

void GPIO_clear_irq_status(gpio_port_name_t gpio) /**Cleans the flag of a determined port when the interruption is done*/
{
	if(GPIO_A == gpio)
	{
		g_intr_status_flag.flag_port_a = FALSE;
	}
	else
	{
		g_intr_status_flag.flag_port_c = FALSE;
	}
}

uint8_t GPIO_get_irq_status(gpio_port_name_t gpio) /**Gets the status of the flag of a port*/
{
	uint8_t status = 0;

	if(GPIO_A == gpio)
	{
		status = g_intr_status_flag.flag_port_a;
	}
	else
	{
		status = g_intr_status_flag.flag_port_c;
	}

	return(status);
}
