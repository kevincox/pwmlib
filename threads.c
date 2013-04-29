#ifndef THREADS_C___PWM_LIBRARY
#define THREADS_C___PWM_LIBRARY

#include "threads.h"

pthread_t pwm_thread;
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

#include <stdlib.h>
#include <sys/io.h>

#include "pwm.h"

unsigned int pwm_port;

void pwm_loop ( pwm_data *d )
{
	for (;;)
	{
		outb(pwm_update_output( d ), pwm_port);
		
		usleep(0);
		//nanosleep(NULL);	//	Give a cancellation point
	}
}

int pwm_start_thread ( pwm_data *d, unsigned int port )
{
	pwm_init(d);
	pwm_port = port;

	return pthread_create(&pwm_thread, NULL, (void*)pwm_loop, d);
}

int pwm_kill_thread ( void )
{
	pthread_cancel(pwm_thread);

	int ret = pthread_join(pwm_thread, NULL); //	Return Value of the thread

	outb(0, pwm_port);

	return ret;
}
#endif
