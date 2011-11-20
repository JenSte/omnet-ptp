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
	PtpPacket* p = new PtpPacket("ptp sync packet", Ptp::Sync);

	// p->ttx is not set, the phy does that for us

	Ieee802Ctrl* c = new Ieee802Ctrl();
	c->setDest(MACAddress::BROADCAST_ADDRESS);
	c->setEtherType(Ptp::Ethertype);
	p->setControlInfo(c);

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
	} else
		delete msg;
}
