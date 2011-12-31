#include "timestampingphy.h"
#include "ptp.h"
#include "ptppacket_m.h"
#include "softwareclock.h"
#include <EtherFrame_m.h>

Define_Module(TimestampingPhy);

void TimestampingPhy::initialize()
{
	enabled = par("enabled");

	clock = SoftwareClock::findFirstClock(getParentModule());
	EV << "using software clock with id " << clock->getId() << '\n';

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
		 && NULL != eth
		 && Ptp::Ethertype == eth->getEtherType()
		 && NULL != ptp) {
			if (ptp->getType() == Ptp::Sync
			 || ptp->getType() == Ptp::Delay_Req) {
				ptp->setTrx(clock->getSWtime());
			}
		}

		send(msg, int_o);
	} else { // message arrived at internal$i
		if (enabled
		 && NULL != eth
		 && Ptp::Ethertype == eth->getEtherType()
		 && NULL != ptp) {
			if (ptp->getType() == Ptp::Sync
			 || ptp->getType() == Ptp::Delay_Req) {
				ptp->setTtx(clock->getSWtime());
			}
		}

		send(msg, ext_o);
	}
}
