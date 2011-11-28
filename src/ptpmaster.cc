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
