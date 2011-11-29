#ifndef PTPSLAVE
#define PTPSLAVE

#include <omnetpp.h>
#include <MACAddress.h>

class SoftwareClock;

class PtpSlave: public cSimpleModule
{
	SoftwareClock* clock;

	MACAddress masterMAC;

	// the timestamps from the last received
	// sync (0/1) and delay_resp(2/3) packets
	struct Timestamps {
		SimTime t[4];
	} timestamps;

	bool controllerEnabled;

	// last measured offset from the master clock
	double offset;

	struct {
		double esum;

		double kp;

		double ki;
	} controller;

	cOutVector offsetVector;

	void sendDelayReq(const MACAddress& masterMAC);

	void correct();

protected:
	void initialize();

	virtual void handleMessage(cMessage* msg);

public:
	PtpSlave();
};

#endif
