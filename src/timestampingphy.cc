/*
    Clock Synchronization Simulation in OMNet++
    Copyright (C) 2012 Jens Steinhauser

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

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
		if (NULL != eth
		 && Ptp::Ethertype == eth->getEtherType()
		 && NULL != ptp) {
			if (ptp->getType() == Ptp::Sync
			 || ptp->getType() == Ptp::Delay_Req) {
				if (enabled)
					ptp->setTrx(clock->getSWtime());

				// this timestamp is always set by the PHY
				ptp->setRealTrx(simTime());
			}
		}

		send(msg, int_o);
	} else { // message arrived at internal$i
		if (NULL != eth
		 && Ptp::Ethertype == eth->getEtherType()
		 && NULL != ptp) {
			if (ptp->getType() == Ptp::Sync
			 || ptp->getType() == Ptp::Delay_Req) {
				if (enabled)
					ptp->setTtx(clock->getSWtime());

				// this timestamp is always set by the PHY
				ptp->setRealTtx(simTime());
			}
		}

		send(msg, ext_o);
	}
}
