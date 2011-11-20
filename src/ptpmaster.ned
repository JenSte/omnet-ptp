package ptp;

simple PtpMaster like IPtpClock
{
	parameters:
		int syncRate = default(10);

	gates:
		inout port;
}
