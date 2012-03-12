#ifndef PTPSLAVE
#define PTPSLAVE

#include <omnetpp.h>
#include <MACAddress.h>

#define CONTROLLER_STAGES 3

class SoftwareClock;

class PtpSlave: public cSimpleModule
{
	SoftwareClock* clock;

	MACAddress masterMAC;

	// the timestamps from the last received
	// sync (0/1) and delay_resp(2/3) packets
	struct Timestamps {
		SimTime msDelay;
		SimTime smDelay;
	} timestamps;

	bool controllerEnabled;

	struct {
		double e[CONTROLLER_STAGES];
		double r[CONTROLLER_STAGES];

		SimTime last;
	} controller;

	cOutVector errorVector[CONTROLLER_STAGES];
	cOutVector offsetVector[CONTROLLER_STAGES];

	cDoubleHistogram msDelay;
	cDoubleHistogram smDelay;

	cOutVector msDelayVector;
	cOutVector smDelayVector;

	cOutVector msDelayRealVector;
	cOutVector smDelayRealVector;

	void sendDelayReq(const MACAddress& masterMAC);

	void correct();

protected:
	void initialize();

	void finish();

	virtual void handleMessage(cMessage* msg);

public:
	PtpSlave();
};

#endif
