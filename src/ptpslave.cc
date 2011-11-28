#include "ptpslave.h"
#include "ptppacket_m.h"
#include "softwareclock.h"

Define_Module(PtpSlave);

void PtpSlave::initialize()
{
	clock = SoftwareClock::findFirstClock(getParentModule());
}

void PtpSlave::sendDelayReq(const MACAddress& masterMAC)
{
	PtpPacket* p = new PtpPacket("ptp delay_req packet", IEEE802CTRL_DATA);
	p->setType(Ptp::Delay_Req);

	// gets overwritten by the phy when hardware
	// timestamping is used
	p->setTtx(clock->getSWtime());

	Ieee802Ctrl* c = new Ieee802Ctrl();
	c->setDest(masterMAC);
	c->setEtherType(Ptp::Ethertype);
	p->setControlInfo(c);

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
