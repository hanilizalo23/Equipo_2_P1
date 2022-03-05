/*
* PIT.h
*
*  Created on: Feb 19, 2022
*  \author PhD. José Luis Pizano Escalante
       \completed by Mauricio Peralta Osorio
*/

#ifndef PIT_H_
#define PIT_H_


#include "stdint.h"
#include "MK64F12.h"


typedef float My_float_pit_t;

/*! This enumerated constant are used to select the PIT to be used*/
typedef enum {PIT_0,PIT_1,PIT_2,PIT_3} PIT_timer_t;


/********************************************************************************************/
/********************************************************************************************/

/********************************************************************************************/
/*!
       \brief      This function contains the ISR for the PIT module
       \param[in]  void.
       \return void
*/
void PIT0_IRQHandler(void);

/********************************************************************************************/
/*!
       \brief      This function configure the PIT to generate a delay base on the system clock.
       It is important to note that this strictly is not device driver since everything is
       contained in a single function,  in general you have to avoid this practices, this only
       for the propose of the homework

       \param[in]  pit_timer channel to be used.
       \param[in]  system_clock system clock use in the K64 (defult = 21e6).
       \param[in]  delay the amount of time the delay the microcontroller
       \return void
*/
void PIT_delay(PIT_timer_t pit_timer, My_float_pit_t system_clock, My_float_pit_t delay);


/********************************************************************************************/
/*!
       \brief      This function enable the clock signal of the pit
       \param[in]  void.
       \return void
*/

void PIT_clock_gating(void);

/********************************************************************************************/
/*!
       \brief      It return the status of the interrupt flag. This flag is a variable created by the programmer.
       It is not the flag related with bit TIF in PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK, this flag must be clear in the ISR of the PIT

       \param[in]  void.
       \return uint8_t flag status
*/
uint8_t PIT_get_interrupt_flag_status(void);

/********************************************************************************************/
/*!
       \brief      Clears the interrupt flag created by the programmer.
       It is not the flag related with bit TIF in PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK, this flag must be clear in the ISR of the PIT

       \param[in]  void.
       \return uint8_t flag status
*/
void PIT_clear_interrupt_flag(void);

/********************************************************************************************/
/*!
       \brief      It enables the PIT

       \param[in]  void.
       \return uint8_t flag status
*/
void PIT_enable(void);

/********************************************************************************************/
/*!
       \brief      It enable the interrupt capabilities of the PIT

       \param[in]  void.
       \return uint8_t flag status
*/
void PIT_enable_interrupt(PIT_timer_t pit);


void PIT_callback_init(void (*handler)(void));


#endif /* PIT_H_ */
