#include "hardwareclockclient.h"
#include "hardwareclock.h"
#include <exception>

HardwareClockClient::HardwareClockClient()
	: _clock(NULL)
{
}

void HardwareClockClient::scheduleAtHWtime(const simtime_t& time, cMessage* msg)
{
	if (NULL == _clock)
		throw std::logic_error("HardwareClockClient::scheduleAtHWtime: no clock set");

	_clock->scheduleAtHWtime(time, msg, this);
}

void HardwareClockClient::scheduleAtInObject(const simtime_t realtime, cMessage* msg)
{
	Enter_Method_Silent();
	take(msg);

	scheduleAt(realtime, msg);
}

void HardwareClockClient::setHardwareClock(HardwareClock* clock)
{
	_clock = clock;
}
