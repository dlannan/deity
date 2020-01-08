// ZEUS timers
//
// Covers both general purpose timers and hardware timers (pc's)
// Performance Counters should be used carefully. Excessive use will result
// in performace degradation
//
// System timer is based on a PC. All timers use this system timer to run.
// You could call it zeus's heartbeat :-)
//
// NOTE: A the minimum accuracy will be limited to 10millisecs - so zeus gets time to 
//		 do its job. The accuracy will be able to be changed - but its not reccommended.

namespace ZEUS
{

	unsigned int SetTimer(void *func, unsigned int period, unsigned int acc)
	{
	}




}
