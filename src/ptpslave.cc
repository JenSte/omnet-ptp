#include "ptpslave.h"
#include "hardwareclock.h"
#include "ptppacket_m.h"

Define_Module(PtpSlave);

void PtpSlave::initialize()
{
	clock = HardwareClock::findFirstClock(getParentModule());
}

void PtpSlave::handleMessage(cMessage* msg)
{
	if (PtpPacket* ptp = dynamic_cast<PtpPacket*>(msg)) {
		// if there is no hardware timestamp by
		// the phy, use a software timestamp
		if (0 == ptp->getTrx().raw())
			ptp->setTrx(clock->getHWtime());

		EV << "delay: " << (ptp->getTrx() - ptp->getTtx()) << '\n';


	}

	delete msg;
}
