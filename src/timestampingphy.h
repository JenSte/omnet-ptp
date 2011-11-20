#ifndef TIMESTAMPINGPHY
#define TIMESTAMPINGPHY

#include <omnetpp.h>

class HardwareClock;

class TimestampingPhy: public cSimpleModule
{
	bool enabled;

	HardwareClock* clock;

	cGate* ext_i;
	cGate* ext_o;
	cGate* int_o;

protected:
	void initialize();

	virtual void handleMessage(cMessage *msg);
};

#endif
