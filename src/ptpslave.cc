#include "ptpslave.h"
#include "ptp.h"
#include "ptppacket_m.h"
#include "softwareclock.h"
#include <Ieee802Ctrl_m.h>

Define_Module(PtpSlave);

void PtpSlave::initialize()
{
	clock = SoftwareClock::findFirstClock(getParentModule());
}

void PtpSlave::sendDelayReq(const MACAddress& masterMAC)
{
	PtpPacket* p = Ptp::newDelayReqPacket(masterMAC);

	// gets overwritten by the phy when hardware
	// timestamping is used
	p->setTtx(clock->getSWtime());

	send(p, "port$o");
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


		EV << "delay: " << (ptp->getTrx() - ptp->getTtx()) << '\n';


		}
	}

	delete msg;
}
