package ptp;

simple Jitter
{
	parameters:
		volatile double delay @unit(s) = default(min(fabs(normal(10e-6s, 20e-6s)), 50e-6s));

	gates:
		inout upperLayer;
		inout lowerLayer;
}
