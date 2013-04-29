// Copyright 2009-2013 Kevin Cox

/*******************************************************************************
*                                                                              *
*  This software is provided 'as-is', without any express or implied           *
*  warranty. In no event will the authors be held liable for any damages       *
*  arising from the use of this software.                                      *
*                                                                              *
*  Permission is granted to anyone to use this software for any purpose,       *
*  including commercial applications, and to alter it and redistribute it      *
*  freely, subject to the following restrictions:                              *
*                                                                              *
*  1. The origin of this software must not be misrepresented; you must not     *
*     claim that you wrote the original software. If you use this software in  *
*     a product, an acknowledgment in the product documentation would be       *
*     appreciated but is not required.                                         *
*                                                                              *
*  2. Altered source versions must be plainly marked as such, and must not be  *
*     misrepresented as being the original software.                           *
*                                                                              *
*  3. This notice may not be removed or altered from any source distribution.  *
*                                                                              *
*******************************************************************************/

/** \mainpage PWM Library
 *
 * \section intro_sec Introduction
 *
 * This PWM library was created to simplify the pwm'ing of the parallel port.
 *
 * \section usage_sec How to Use
 *
 * \subsection files Files
 *      Main Header: pwm/pwm.h
 *		Threading support: pwm/threads.h
 **/

/** \file pwm.h
    \brief Main header file
*/

#ifndef PWM_H___PWM_LIBRARY
#define PWM_H___PWM_LIBRARY

#include <stdint.h>

#include "internals.h"

typedef struct _pwm_data
{
	uint8_t state; // Integer to hold the state of PWM's
	unsigned short int on[8]; // Whether we are PWM'ing the bit
	float load[8]; // The load percent
	float hz[8];   // Not truly Hz but instead how many microseconds per cycle

	struct timeval next[8];
	//int nexts[8];      // Next time to change the bit
	//uint32_t nextu[8]; //
} pwm_data;

/// Initates the PWM structure
/**
 * This must be called before any other functions.  If called afterwords it
 * will overwrite your settings.  If not called the results are undefined.
 * 
 * \note pwm_start_thread() calls this for you.
 * 
 * \param d a pwm_data structure that has been initilized bypwm_init().
 **/
void pwm_init( pwm_data *d );

/// Update the current output.
/**
 * Updates the current pwm output to the port.
 * 
 * \param d a pwm_data structure that has been initialized bypwm_init().
 * 
 * \return An 8 bit mask of the current output.
 */
uint8_t pwm_update_output( pwm_data *d );

/// Starts or stops PWM'ing \a bit
/** 
 * Start or stop PWM'ing \a bit.
 * 
 * \param d a pwm_data structure that has been initialized bypwm_init().
 * \param bit The bit to affect.
 * \param state Something that evaluates to \c TRUE for on or \c FALSE for off.
 */
void pwm_set_onoff( pwm_data *d, unsigned int bit, int state );

/// Set the state
/**
 * Sets the state of \a bit.
 * 
 * \note If \a bit is currently being PWM'ed, it will only stay in \a state for
 *   one cycle.
 * 
 * \param d a pwm_data structure that has been initialized bypwm_init().
 * \param bit The \a bit to set the state of.
 * \param state The state to set the bit.  Any value that evaluates to true
 *   will set the bit high and any that evaluate false will set the bit low.
 */
void pwm_set_state( pwm_data *d, unsigned int bit, int state );
void pwm_get_state( pwm_data *d, unsigned int bit );

/// Send a pulse from \a bit.
/** 
 * Send a pulse of length \a len from \a bit.
 *
 * \param d a pwm_data structure that has been initialized bypwm_init().
 * \param bit The bit to send the pulse from.
 * \param len The desired length of the pulse (in µs (microseconds))
 * \param immediately If this is very urgent.  If this is set, if the bit is
 *   \c HIGH if will be set \c LOW where it will be set \c HIGH again to send
 *   the pulse during the next call of pwm_update_output().
 *   \warning The gap between setting the bit from \c HIGH to \c LOW may be
 *     to quick for the hardware to change or for the device to register.
 *     Use immediately with caution and preferably not at all.
 */
void pwm_send_pulse ( pwm_data *d, unsigned int bit, unsigned long int len, unsigned short int imediatly );

/// Set the frequency.
/**
 * Set the desired frequency for bit 'bit'.
 * 
 * \param d a pwm_data structure that has been initialized bypwm_init().
 * \param bit \a Bit to set frequency for.
 * \param hertz The desired frequency. (in HZ)
 */
void pwm_set_frequency( pwm_data *d, unsigned short int bit, float hertz );

/// Set the duty cycle
/**
 * Sets the duty cycle for \a bit.
 * 
 * \warning DO NOT use this function to turn off a bit.  While setting duty
 * cycle to 0 or 1 will effectively turn on or off the bit it will continue to
 * use resources.  Instead use #pwm_set_onoff() and #pwm_set_state().
 * 
 * \param d a pwm_data structure that has been initialized bypwm_init().
 * \param bit The \a bit to set the duty cycle for
 * \param percent The desired duty cycle for \a bit.  This is a number in the
 * range of 0 to 1 representing the amount of time \b HIGH\b .
 */
void pwm_set_load( pwm_data *d, unsigned int bit, double per );

#endif
