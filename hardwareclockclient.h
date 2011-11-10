#ifndef HARDWARECLOCKCLIENT_H
#define HARDWARECLOCKCLIENT_H

//#include "hardwareclock.h"
#include <omnetpp.h>

class HardwareClock;

/// \brief Base class for classes that want to schedule events at hardware time.
class HardwareClockClient: public cSimpleModule
{
	/// Hardware clock to use in 'scheduleAtHWtime()'.
	HardwareClock* _clock;

	/// Calls 'scheduleAt' in the context of the HardwareClockClient object
	/// (calls Enter_Method_Silent()).
	///
	/// \param realtime	The simulation time when to schedule the message.
	/// \param msg	The message to schedule. The ownership of the message is
	/// 		take()n in the function.
	void scheduleAtInObject(const simtime_t realtime, cMessage* msg);

protected:
	/// Sets the hardware clock to use in 'scheduleAtHWtime()'.
	void setHardwareClock(HardwareClock* clock);

	/// Schedules a message at hardware time.
	///
	/// The hardware clock whose time is used has to be set with
	/// 'setHardwareClock()' prior to calling this function.
	/// Because the message is scheduled internally through the
	/// objects 'scheduleAt' function, it's really a self message
	/// (and msg->isSelfMessage() can be used).
	///
	/// \param hwtime	The hardware time when to schedule the message.
	/// \param msg	The message to schedule.
	void scheduleAtHWtime(const simtime_t& hwtime, cMessage* msg);

public:
	HardwareClockClient();

	// needs access to 'scheduleAtInObject'
	friend class HardwareClock;
};

#endif
