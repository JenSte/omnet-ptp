#ifndef PTPSLAVE
#define PTPSLAVE

#include <omnetpp.h>

class SoftwareClock;

class PtpSlave: public cSimpleModule
{
	SoftwareClock* clock;

protected:
	void initialize();

	virtual void handleMessage(cMessage* msg);
};

#endif
