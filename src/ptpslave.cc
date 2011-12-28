#include "ptpslave.h"
#include "ptp.h"
#include "ptppacket_m.h"
#include "softwareclock.h"
#include <Ieee802Ctrl_m.h>

Define_Module(PtpSlave);

PtpSlave::PtpSlave()
{
	controller.kp  = 0.7;
	controller.kp2 = 0.7; 
	controller.ti1 = 0.4;
	controller.ti2 = 1;
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

	tpVector.setName("p part");
	tpVector.setUnit("s");

	tiVector.setName("i1 part");
	tiVector.setUnit("s");
	
	ti2Vector.setName("i2 part");
	ti2Vector.setUnit("s");

	controllerEnabled = par("controllerEnabled");

	controller.esum = 0.0;
	controller.esum2 = 0.0;
	controller.last = SimTime();

	WATCH(controller.esum);
	WATCH(controller.kp);
//	WATCH(controller.ki);
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

//	double Ta = (simTime() - controller.last).dbl();
//	controller.last = simTime();
	double Ta = 0.1;
//	EV << "Ta: " << Ta << '\n';

	double tp = controller.kp * e;
	double ti1 = Ta / controller.ti1 * controller.esum;

	controller.esum2 += ti1;

	double tp2 = controller.kp2 * ti1;
	double ti2 = Ta / controller.ti2 * controller.esum2;

	tpVector.record(tp);
	tiVector.record(ti1);
	ti2Vector.record(ti2);

	double y = tp + tp2 + ti2;

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
