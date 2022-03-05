/**
	\file
	\brief
			This is the source file for the colors needed in the main implementation.
	\author Nelida Paulina Hernández Moya
	\date	15/02/2022
 */

#ifndef RGB_H_
#define RGB_H_

/**Setters for the sequence and color*/
void SW2_cb(void);
void SW3_cb(void);

/**Function that is called from callback for the functioning of the PIT*/
void PIT_cb(void);

/**Getters for the sequence and color*/
uint8_t get_sequence(void);
uint8_t get_color(void);

void green(void); 	/**Function that turn on the green LED, waiting and turning it off*/
void blue(void); 	/**Function that turn on the blue LED, waiting and turning it off*/
void purple(void);	/**Function that turn on the purple LED, waiting and turning it off*/
void red(void);		/**Function that turn on the red LED, waiting and turning it off*/
void yellow(void);	/**Function that turn on the yellow LED, waiting and turning it off*/
void white(void);   /**Function that turn on the white LED, waiting and turning it off*/
void no_color(void); /**Functions that doesn't turn on anything**/



#endif /* RGB_H_ */
