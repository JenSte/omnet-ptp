#include "driftsource.h"

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
