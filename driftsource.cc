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

BoundedDrift::BoundedDrift(const cPar& distribution)
	: distribution(distribution)
{
}

double BoundedDrift::nextValue()
{
	return distribution.doubleValue();
}

BoundedDriftVariation::BoundedDriftVariation(const cPar& distribution, double max_drift_variation, const simtime_t& tint)
	: BoundedDrift(distribution)
	, max_drift_change(tint.dbl() * max_drift_variation)
	, last_drift(0.0)
{
}

double BoundedDriftVariation::nextValue()
{
	double drift = BoundedDrift::nextValue();

	double diff = drift - last_drift;

	// limit the drift
	     if (diff >  max_drift_change)
		drift = last_drift + max_drift_change;
	else if (diff < -max_drift_change)
		drift = last_drift - max_drift_change;

	last_drift = drift;
	return drift;
}
