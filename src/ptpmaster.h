#ifndef PTPMASTER
#define PTPMASTER

#include <omnetpp.h>

class PtpMaster: public cSimpleModule
{
protected:
	void initialize();

	virtual void handleMessage(cMessage* msg);
};

#endif
