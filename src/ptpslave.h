#ifndef PTPSLAVE
#define PTPSLAVE

#include <omnetpp.h>
#include <MACAddress.h>

class SoftwareClock;

class PtpSlave: public cSimpleModule
{
	SoftwareClock* clock;

	MACAddress masterMAC;

	void sendDelayReq(const MACAddress& masterMAC);

protected:
	void initialize();

	virtual void handleMessage(cMessage* msg);
};

#endif
