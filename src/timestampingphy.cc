#include "timestampingphy.h"
#include "hardwareclock.h"
#include "ptp.h"
#include "ptppacket_m.h"
#include <EtherFrame_m.h>

Define_Module(TimestampingPhy);

void TimestampingPhy::initialize()
{
	enabled = par("enabled");

	// use the first available clock, if there is one
	clock = NULL;
	std::vector<HardwareClock*> clocks = HardwareClock::findClocks(getParentModule());
	if (clocks.size() > 0)
		clock = clocks[0];

	if (enabled && NULL == clock)
		EV << "timestamping is enabled, but no clock was found\n";

	ext_i = gate("external$i");
	ext_o = gate("external$o");
	int_o = gate("internal$o");
}

void TimestampingPhy::handleMessage(cMessage *msg)
{
	EthernetIIFrame* eth = dynamic_cast<EthernetIIFrame*>(msg);
	PtpPacket* ptp = NULL;
	if (eth) {
		// if needed, creates a copy of the shared encapsulated packet
		// and returns a pointer to the new one
		ptp = dynamic_cast<PtpPacket*>(eth->getEncapsulatedPacket());
	}

	if (ext_i == msg->getArrivalGate()) {
		if (enabled
		 && NULL != clock
		 && NULL != eth
		 && Ptp::Ethertype == eth->getEtherType()
		 && NULL != ptp) {
			ptp->setTrx(clock->getHWtime());
		}

		send(msg, int_o);
	} else { // message arrived at internal$i
		if (enabled
		 && NULL != clock
		 && NULL != eth
		 && Ptp::Ethertype == eth->getEtherType()
		 && NULL != ptp) {
			ptp->setTtx(clock->getHWtime());
		}

		send(msg, ext_o);
	}
}
