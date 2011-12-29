#include "ptpslave.h"
#include "ptp.h"
#include "ptppacket_m.h"
#include "softwareclock.h"
#include <Ieee802Ctrl_m.h>
#include <sstream>

Define_Module(PtpSlave);

PtpSlave::PtpSlave()
{
	controller.r[0] =  1;
	controller.r[1] = 12.5;
	controller.r[2] = 25;
}

void PtpSlave::initialize()
{
	clock = SoftwareClock::findFirstClock(getParentModule());

	WATCH(timestamps.t[0]);
	WATCH(timestamps.t[1]);
	WATCH(timestamps.t[2]);
	WATCH(timestamps.t[3]);

	for (int i = 0; i < CONTROLLER_STAGES; i++) {
		std::stringstream sse;
		std::stringstream sso;

		sse << "e " << i;
		sso << "o " << i;

		errorVector[i].setName(sse.str().c_str());
		errorVector[i].setUnit("s");

		offsetVector[i].setName(sso.str().c_str());
		offsetVector[i].setUnit("s");
	}

	controllerEnabled = par("controllerEnabled");
	memset(controller.e, 0, sizeof(controller.e));
	controller.last = SimTime();
}

void PtpSlave::sendDelayReq(const MACAddress& masterMAC)
{
	PtpPacket* p = Ptp::newDelayReqPacket(masterMAC);

	// gets overwritten by the phy when hardware
	// timestamping is used
	p->setTtx(clock->getSWtime());

	send(p, "port$o");
}

void PtpSlave::correct()
{
	double offset = ( (timestamps.t[1] - timestamps.t[0])
	                - (timestamps.t[3] - timestamps.t[2]) ).dbl() / 2.0;

	if (!controllerEnabled)
		return;

	//
	// controller
	//

//	double Ta = (simTime() - controller.last).dbl();
//	controller.last = simTime();
	double Ta = 0.1;

	// offset sum
	double os = 0.0;

	for (int i = CONTROLLER_STAGES - 1; i >= 0; i--) {
		if (0 == i)
			controller.e[0]  = -offset;
		else
			controller.e[i] += controller.e[i-1] * Ta;

		errorVector[i].record(controller.e[i]);

		double o = controller.e[i] * controller.r[i];
		offsetVector[i].record(o);

		os += o;
	}

	clock->setFactor(os);
}

void PtpSlave::handleMessage(cMessage* msg)
{
	if (PtpPacket* ptp = dynamic_cast<PtpPacket*>(msg)) {
		switch (ptp->getType()) {
		case Ptp::Sync:
			// if there is no hardware timestamp by
			// the phy, use a software timestamp
			if (0 == ptp->getTrx().raw())
				ptp->setTrx(clock->getSWtime());

			// if this is the first sync packet received
			// from a master clock, use it from now on
			if (masterMAC.isUnspecified()) {
				Ieee802Ctrl* c = check_and_cast<Ieee802Ctrl*>(ptp->getControlInfo());

				masterMAC = c->getSrc();
			}

			sendDelayReq(masterMAC);

			timestamps.t[0] = ptp->getTtx();
			timestamps.t[1] = ptp->getTrx();

			break;

		case Ptp::Delay_Resp:
			timestamps.t[2] = ptp->getTtx();
			timestamps.t[3] = ptp->getTrx();

			correct();

			break;

		default:
			break;
		}
	}

	delete msg;
}
