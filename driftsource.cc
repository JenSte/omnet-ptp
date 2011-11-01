#include "driftsource.h"
#include <omnetpp.h>

Driftsource::~Driftsource()
{
}

ConstantDrift::ConstantDrift(double drift)
	: drift(drift)
{
}

double ConstantDrift::nextValue()
{
	return drift;
}

BoundedDrift::BoundedDrift(const cPar& distribution)
	: distribution(distribution)
{
}

double BoundedDrift::nextValue()
{
	return distribution.doubleValue();
}
