/**
	\file
	\brief This is the source file for the colors needed in the main implementation.
	\author Nelida Paulina Hernández Moya
	\date	15/02/2022
 */

#ifndef RGB_H_
#define RGB_H_

void rgb_init(void); /**Helps to do all the initialization of the GPIOs that corresponds to the RGB LED*/

void red_on(void); /**Turn on the red LED*/
void blue_on(void); /**Turn on the blue LED*/
void green_on(void); /**Turn on the green LED*/
void purple_on(void); /**Turn on the purple LED*/
void yellow_on(void); /**Turn on the yellow LED*/
void white_on(void); /**Turn on the white LED*/

void red_off(void); /**Turn off the red LED*/
void blue_off(void); /**Turn off the blue LED*/
void green_off(void); /**Turn off the green LED*/
void purple_off(void); /**Turn off the purple LED*/
void yellow_off(void); /**Turn off the yellow LED*/
void white_off(void); /**Turn off the white LED*/

void rgb_basic_off(void); /**Turn off the three colors of the LED*/

#endif /* RGB_H_ */
