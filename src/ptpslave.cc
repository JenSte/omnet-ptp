#include "ptpslave.h"
#include "ptppacket_m.h"
#include "softwareclock.h"

Define_Module(PtpSlave);

void PtpSlave::initialize()
{
	clock = SoftwareClock::findFirstClock(getParentModule());
}

void PtpSlave::handleMessage(cMessage* msg)
{
	if (PtpPacket* ptp = dynamic_cast<PtpPacket*>(msg)) {
		// if there is no hardware timestamp by
		// the phy, use a software timestamp
		if (0 == ptp->getTrx().raw())
			ptp->setTrx(clock->getSWtime());

		EV << "delay: " << (ptp->getTrx() - ptp->getTtx()) << '\n';


	}

	delete msg;
}
