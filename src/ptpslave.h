#ifndef PTPSLAVE
#define PTPSLAVE

#include <omnetpp.h>

class HardwareClock;

class PtpSlave: public cSimpleModule
{
	HardwareClock* clock;

protected:
	void initialize();

	virtual void handleMessage(cMessage* msg);
};

#endif
