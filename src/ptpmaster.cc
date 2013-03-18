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

#include "ptpmaster.h"
#include "ptp.h"
#include "ptppacket_m.h"
#include <Ieee802Ctrl_m.h>

Define_Module(PtpMaster);

PtpMaster::PtpMaster()
	: selfMsg(NULL)
{
}

void PtpMaster::sendSync()
{
	PtpPacket* p = Ptp::newSyncPacket(MACAddress::BROADCAST_ADDRESS);

	// p->ttx is not set, the phy does that for us

	send(p, "port$o");
}

void PtpMaster::answerDelayReq(const PtpPacket* delayReq)
{
	Ieee802Ctrl* c = check_and_cast<Ieee802Ctrl*>(delayReq->getControlInfo());

	MACAddress slaveMAC = c->getSrc();

	PtpPacket* p = Ptp::newDelayRespPacket(slaveMAC);

	// just send back the timestamps
	// (the timestamping phy doesn't overwrite timestamps
	// in a Delay_Resp packet)
	p->setTtx(delayReq->getTtx());
	p->setTrx(delayReq->getTrx());
	p->setRealTtx(delayReq->getRealTtx());
	p->setRealTrx(delayReq->getRealTrx());

	send(p, "port$o");
}

void PtpMaster::initialize()
{
	if (NULL == selfMsg)
		selfMsg = new cMessage("ptpMaster self message");

	syncInterval = SimTime::parse("1s") / (int)par("syncRate");

	scheduleAt(syncInterval, selfMsg);
}

void PtpMaster::handleMessage(cMessage* msg)
{
	if (selfMsg == msg) {
		sendSync();

		scheduleAt(simTime() + syncInterval, msg);
	} else {
		if (PtpPacket* ptp = dynamic_cast<PtpPacket*>(msg)) {
			if (Ptp::Delay_Req == ptp->getType())
				answerDelayReq(ptp);
		}

		delete msg;
	}
}
