#include "ptp.h"
#include "ptppacket_m.h"
#include <Ieee802Ctrl_m.h>

static PtpPacket* newPtpPacket(const MACAddress& dest, Ptp::FrameType type)
{
	const char* name = NULL;
	int64 length = 0;

	// the length values are taken from section D.1.3.2 and D.1.3.4 of the standard

	switch (type) {
	case Ptp::Sync:       name = "ptp sync packet";       length = 126; break;
	case Ptp::Delay_Req:  name = "ptp delay_req packet";  length = 126; break;
	case Ptp::Delay_Resp: name = "ptp delay_resp packet"; length =  63; break;
	}

	PtpPacket* p = new PtpPacket(name, IEEE802CTRL_DATA);
	p->setByteLength(length);
	p->setType(type);

	Ieee802Ctrl* c = new Ieee802Ctrl();
	c->setDest(dest);
	c->setEtherType(Ptp::Ethertype);
	p->setControlInfo(c);

	return p;
}

PtpPacket* Ptp::newSyncPacket(const MACAddress& dest)
{
	return newPtpPacket(dest, Ptp::Sync);
}

PtpPacket* Ptp::newDelayReqPacket(const MACAddress& dest)
{
	return newPtpPacket(dest, Ptp::Delay_Req);
}

PtpPacket* Ptp::newDelayRespPacket(const MACAddress& dest)
{
	return newPtpPacket(dest, Ptp::Delay_Resp);
}
