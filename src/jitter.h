#ifndef JITTER_H
#define JITTER_H

#include <omnetpp.h>

class Jitter: public cSimpleModule
{
	cGate* u_i;
	cGate* u_o;
	cGate* l_o;

	SimTime last;

	cDoubleHistogram jitterHistogram;

protected:
	void initialize();

	void finish();

	void handleMessage(cMessage* msg);
};

#endif
