#include "Delay_5110.h"

void Delayus(volatile u32 us) 
{
	u32 i,time;
	time = (u32)(SYSTEMFREQ / 1000000.0 * us);
	for(i = 0;i < time;i++) 
		__nop();
}
void Delayms(volatile u32 ms) 
{
	u32 i,time;
	time = (u32)(SYSTEMFREQ / 1000.0 * ms);
	for(i = 0;i < time;i++) 
	{
		__nop();
	}
}
