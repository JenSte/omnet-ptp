#ifndef PTPSLAVE
#define PTPSLAVE

#include <omnetpp.h>

class PtpSlave: public cSimpleModule
{
protected:
	void initialize();

	virtual void handleMessage(cMessage* msg);
};

#endif
