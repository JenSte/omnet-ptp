#include "ptp.h"
#include "ptppacket_m.h"
#include <Ieee802Ctrl_m.h>

static PtpPacket* newPtpPacket(const MACAddress& dest, Ptp::FrameType type)
{
	const char* name = NULL;

	switch (type) {
	case Ptp::Sync:       name = "ptp sync packet";       break;
	case Ptp::Delay_Req:  name = "ptp delay_req packet";  break;
	case Ptp::Delay_Resp: name = "ptp delay_resp packet"; break;
	}

	PtpPacket* p = new PtpPacket(name, IEEE802CTRL_DATA);
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
