package ptp;

simple TimestampingPhy
{
	parameters:
		bool enabled = default(true);

	gates:
		inout internal;
		inout external;
}
