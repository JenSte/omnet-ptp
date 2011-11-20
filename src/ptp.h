#ifndef PTP_H
#define PTP_H

namespace Ptp
{
	enum Frames {
		Sync,
		Delay_Req,
		Delay_Resp
	};

	enum ET { Ethertype = 0x88f7 };
}

#endif
