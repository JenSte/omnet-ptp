#include "ptpslave.h"
#include "hardwareclock.h"
#include "ptppacket_m.h"
#include <exception>

Define_Module(PtpSlave);

void PtpSlave::initialize()
{
	std::vector<HardwareClock*> clocks = HardwareClock::findClocks(getParentModule());
	if (0 == clocks.size())
		throw std::logic_error("no HardwareClock found");
	clock = clocks[0];
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
