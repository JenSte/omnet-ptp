#ifndef TIMESTAMPINGPHY
#define TIMESTAMPINGPHY

#include <omnetpp.h>

class TimestampingPhy: public cSimpleModule
{
	cGate* ext_i;
	cGate* ext_o;
	cGate* int_o;

protected:
	void initialize();

	virtual void handleMessage(cMessage *msg);
};

#endif
