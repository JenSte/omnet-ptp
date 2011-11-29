#include "ptpslave.h"
#include "ptp.h"
#include "ptppacket_m.h"
#include "softwareclock.h"
#include <Ieee802Ctrl_m.h>

Define_Module(PtpSlave);

PtpSlave::PtpSlave()
{
	controller.kp = 0.45;
	controller.ki = 2.6;
}

void PtpSlave::initialize()
{
	clock = SoftwareClock::findFirstClock(getParentModule());

	WATCH(timestamps.t[0]);
	WATCH(timestamps.t[1]);
	WATCH(timestamps.t[2]);
	WATCH(timestamps.t[3]);

	offset = 0.0;
	WATCH(offset);

	offsetVector.setName("offset");
	offsetVector.setUnit("s");

	controllerEnabled = par("controllerEnabled");

	controller.esum = 0.0;

	WATCH(controller.esum);
	WATCH(controller.kp);
	WATCH(controller.ki);
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
	offset = ( (timestamps.t[1] - timestamps.t[0])
	         - (timestamps.t[3] - timestamps.t[2]) ).dbl() / 2.0;

	offsetVector.record(offset);

	if (!controllerEnabled)
		return;

	// PI controller
	double e = -offset;

	controller.esum += e;

	double Ta = 0.1;

	double y = controller.kp * e + controller.ki * Ta * controller.esum;

	clock->setFactor(y);
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
