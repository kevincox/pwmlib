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

/** \file threads.h
 * \brief Threading header file
 *
 * This file will allow users to use threading so that they don't have to worry
 * about calling pwm_update_output() manually.  This does the pwm to a parallel
 * port or GPIO pins.
 *
 * \note This file must be compiled and linked with -pthread.
 */

#ifndef THREADS_H___PWM_LIBRARY
#define THREADS_H___PWM_LIBRARY

#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

#include "pwm.h"

/** \internal
 * \brief Calls pwm_update_output() continusly
 *
 * An internal function that is used as the thread function for
 * pwm_start_thread().  It calls pwm_update_output() over and over again.
 */
void pwm_loop ( pwm_data *d );

/// Starts the background thread to update the output
/**
 * \note It is not nessary to call pwm_int() when calling this function.
 * \warning All settings will be lost when calling this function
 *   (Because it calls pwm_init())
 *
 * \param port port to PWM
 *
 * \return Returns 0 on success and a value that evaluates to false on error
 */
int pwm_start_thread ( pwm_data *d, unsigned int port );

/// Kills the background thread
/** Kills the background pwm thread created by start_pwm_thread()
 *
 * \warning You must call pwm_start_thread() before this function and since the
 *   last call of this function.
 */
int pwm_kill_thread ( void );

#endif
