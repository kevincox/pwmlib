# PWM Lib

A library I wrote for a series of projects in a computer engineering class.
They were designed for a parallel port but the output byte can be put anywhere.

It has been tested to be very accurate and the true accuracy would be limited
by the speed of the system and the accuracy of the clock.

All bits are handled separately so they can have different frequencies/loads
and the offsets are not going to be in sync even if they have the same
frequency. 
