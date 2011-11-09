#ifndef SCHEDULETESTAPP_H
#define SCHEDULETESTAPP_H

#include <omnetpp.h>

class HardwareClock;

/// Test application for the hardware clock implementation.
class ScheduleTestApp: public cSimpleModule
{
	/// The number of pending test messages.
	int pending;

	struct ScheduleTestMsg: public cMessage
	{
		HardwareClock* clock;

		simtime_t time;
	};

protected:
	/// Handles the given message.
	virtual void handleMessage(cMessage *msg);

	int numInitStages() const;

	void initialize(int stage);

	void finish();
};

#endif
