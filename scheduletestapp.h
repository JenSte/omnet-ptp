#ifndef SCHEDULETESTAPP_H
#define SCHEDULETESTAPP_H

#include <omnetpp.h>

class HardwareClock;

struct ScheduleTestMsg: public cMessage
{
	HardwareClock* clock;

	simtime_t time;
};

class ScheduleTestApp: public cSimpleModule
{
	/// The number of pending test messages.
	int pending;

protected:
	/// Handles the given message.
	virtual void handleMessage(cMessage *msg);

	int numInitStages() const;

	void initialize(int stage);

	void finish();
};

Define_Module(ScheduleTestApp);

#endif
