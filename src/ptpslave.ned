package ptp;

simple PtpSlave like IPtpClock
{
	parameters:
		bool controllerEnabled = default(true);

	gates:
		inout port;
}
