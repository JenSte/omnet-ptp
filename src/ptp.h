#ifndef PTP_H
#define PTP_H

class PtpPacket;
class MACAddress;

namespace Ptp
{
	enum FrameType {
		Sync,
		Delay_Req,
		Delay_Resp
	};

	enum ET { Ethertype = 0x88f7 };

	PtpPacket* newSyncPacket(const MACAddress& dest);

	PtpPacket* newDelayReqPacket(const MACAddress& dest);

	PtpPacket* newDelayRespPacket(const MACAddress& dest);
}

#endif
