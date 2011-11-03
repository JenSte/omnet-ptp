#ifndef SCHEDULETESTAPP_H
#define SCHEDULETESTAPP_H

#include <omnetpp.h>

class HardwareClock;

class ScheduleTestApp: public cSimpleModule
{
	std::vector<HardwareClock*> clocks;

protected:
	/// Initializes the module.
	virtual void initialize();

	/// Handles the given message.
	virtual void handleMessage(cMessage *msg);
};

Define_Module(ScheduleTestApp);

#endif
