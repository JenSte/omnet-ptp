cplusplus {{
#include "simtime.h"
#include "ptp.h"
}};

class noncobject SimTime;
class noncobject Ptp::FrameType;

packet PtpPacket
{
	Ptp::FrameType type;

	SimTime ttx;
	SimTime trx;
}
