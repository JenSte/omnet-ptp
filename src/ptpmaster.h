#ifndef PTPMASTER
#define PTPMASTER

#include <omnetpp.h>

class PtpPacket;

class PtpMaster: public cSimpleModule
{
	SimTime syncInterval;

	cMessage* selfMsg;

	void sendSync();

	void answerDelayReq(const PtpPacket* delayReq);

protected:
	void initialize();

	virtual void handleMessage(cMessage* msg);

public:
	PtpMaster();
};

#endif
