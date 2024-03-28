/* Template app on which you can build your own. */
#pragma GCC optimize( "O3" )
#define BREAK() __asm( "C.EBREAK" ) // software breakpoint

#include "ch32v003fun.h"
#include <stdio.h>
#include <string.h>

static char aIsPrime[1000];
static unsigned int NumPrimes;

/*********************************************************************
 *
 *       _CalcPrimes()
 */
// uncomment the following prototype to run this fn from SRAM
// static void _CalcPrimes( unsigned int ) __attribute__( ( section( ".srodata" ) ) ) __attribute__( ( used ) );
static void _CalcPrimes( unsigned int NumItems )
{
	unsigned int i;
	unsigned int j;

	//
	// Mark all as potential prime numbers
	//
	memset( aIsPrime, 1, NumItems );
	//
	// 2 deserves a special treatment
	//
	for ( i = 4; i < NumItems; i += 2 )
	{
		aIsPrime[i] = 0; // Cross it out: not a prime
	}
	//
	// Cross out multiples of every prime starting at 3. Crossing out starts at i^2.
	//
	for ( i = 3; i * i < NumItems; i++ )
	{
		if ( aIsPrime[i] )
		{
			j = i * i; // The square of this prime is the first we need to cross out
			do
			{
				aIsPrime[j] = 0; // Cross it out: not a prime
				j += 2 * i; // Skip even multiples (only 3*, 5*, 7* etc)
			} while ( j < NumItems );
		}
	}
	//
	// Count prime numbers
	//
	NumPrimes = 0;
	for ( i = 2; i < NumItems; i++ )
	{
		if ( aIsPrime[i] )
		{
			NumPrimes++;
		}
	}
}

int main( void )
{
	SystemInit(); // sets up systick at 6MHz (among other things)
	volatile unsigned int Cnt = 0;

	while ( 1 )
	{
		Cnt = 0;
		SysTick->CNT = 0;

		while ( ( (int32_t)( SysTick->CNT - 6000000 ) ) < 0 ) // counting to 6000000 at 6MHz is 1 second
		{
			_CalcPrimes( sizeof( aIsPrime ) );
			Cnt++;
		}
		BREAK(); // look at Cnt with your debugger
	}
}
