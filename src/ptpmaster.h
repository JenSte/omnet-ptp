#ifndef PTPMASTER
#define PTPMASTER

#include <omnetpp.h>

class PtpMaster: public cSimpleModule
{
	SimTime syncInterval;

	cMessage* selfMsg;

	void sendSync();

protected:
	void initialize();

	virtual void handleMessage(cMessage* msg);

public:
	PtpMaster();
};

#endif
