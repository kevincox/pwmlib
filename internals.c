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

#ifndef PWM_INTERNALS_C___PWM_LIBRARY
#define PWM_INTERNALS_C___PWM_LIBRARY

#include "internals.h"

/** \internal
	\brief Wrap the extra microseconds into the seconds value

	If the microseconds value is greater than \c 1 \c 000 \c 000 it will be
	lowered and the appropriate value will be added to the seconds.
*/
void _pwm_normalize_time ( struct timeval *t )
{
	while ( t->tv_usec >= 1000000 ) // If the microseconds get overflowed
	{
		t->tv_usec -= 1000000; //	Take off 1 second
		t->tv_sec++;           // And add it to the seconds
	}
}

#endif
