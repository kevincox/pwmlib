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

#ifndef PWM_C___PWM_LIBRARY
#define PWM_C___PWM_LIBRARY

#include <stdlib.h>
#include <sys/time.h>

#include "pwm.h"

uint8_t pwm_update_output ( pwm_data *d )
{
	int i;
	int w; // Worker
	struct timeval time;

	for ( i = 7; i >= 0; i-- )
	{
		if (!d->on[i]) continue; //	We are not pwm'ing the led

		gettimeofday(&time, NULL);
		//printf("%d.%d\t", d->next.tv_sec, d->next.tv_usec);

		if (  time.tv_sec > d->next[i].tv_sec ||
		    ( time.tv_sec == d->next[i].tv_sec && time.tv_usec > d->next[i].tv_usec )
		   )
		{
			w = 1 << i; //	But a 1 in the right column

			if ( w & d->state ) //	Bit is on turn if off
			{
				if ( d->on[i] == 2 ) pwm_set_onoff(d, i, 0);
				d->next[i].tv_usec += (1.0 - d->load[i]) * d->hz[i]; //	How long it should stay off for
				d->state &= 255 ^ (1 << i); //	If it is a one make it zero
			}
			else
			{
				d->next[i].tv_usec += d->load[i] * d->hz[i]; //	How long it should stay on for
				d->state |= w; //	If it is zero make it one
			}

		_pwm_normalize_time(&d->next[i]);
		}
	}
	
	return d->state;
}

void pwm_set_frequency ( pwm_data *d, unsigned short int bit, float hertz )
{
	d->hz[bit] = 1000000.0 / hertz; // Convert Hz to microseconds
}

void pwm_set_load ( pwm_data *d, unsigned int bit, double percent )
{
	d->load[bit] = percent;
}

void pwm_set_state ( pwm_data *d, unsigned int bit, int state )
{
	int w = 1 << bit; //	But a 1 in the right column

	struct timeval time;
	gettimeofday(&time, NULL);

	d->next[bit].tv_usec = time.tv_sec;

	if (state) //	Bit is to be turned off
	{
		d->next[bit].tv_usec = time.tv_sec + d->load[bit] * d->hz[bit]; //	How long it should stay on for
		d->state |= w; //	If it is zero make it one
	}
	else
	{
		d->next[bit].tv_usec = time.tv_sec + (1.0 - d->load[bit]) * d->hz[bit]; //	How long it should stay off for
		d->state &= 255 ^ (w); //	If it is a one make it zero
	}


	_pwm_normalize_time(&d->next[bit]);
}

void pwm_send_pulse ( pwm_data *d, unsigned int bit, unsigned long int len, unsigned short int imediatly )
{
	int w = 1 << bit;     // But a 1 in the right column
	d->on[bit] = 2;      // Turn if on and tell it we're pulsing
	d->load[bit] = 0.9;  // Big number so if we need to start off there won't be a long delay
	d->hz[bit] = len / d->load[bit]; // Get the right amount of time

	struct timeval time;
	gettimeofday(&time, NULL);

	if ( w & d->state ) //	Bit is on, we need it off before pulsing
	{
		if (imediatly)
		{
			pwm_set_state ( d, bit, 0 );
			d->next[bit].tv_sec = time.tv_sec; // Change NOW
			d->next[bit].tv_usec = time.tv_usec;
		}
	}
	else
	{
		pwm_set_state ( d, bit, 1 );
	}
}

void pwm_set_onoff ( pwm_data *d, unsigned int bit, int state )
{
	d->on[bit] = state != 0;

	if (state)
	{
		struct timeval time;
		gettimeofday(&time, NULL);

		d->next[bit].tv_sec = time.tv_sec;
		d->next[bit].tv_usec = time.tv_usec;
	}
}

int pwm_get_onoff ( pwm_data *d, unsigned int bit )
{
	return d->on[bit];
}

void pwm_init ( pwm_data *d )
{
	int i;

	d->state = 0;

	for ( i = 0; i < 8; i++ )
	{
		d->load[i] = 0; //	Init load
		d->on[i] = 0; //	Init the state to off
		pwm_set_frequency(d, i, 30);  // Init frequency

		pwm_set_onoff(d, i, 0); // Turn off
	}
}

#endif
