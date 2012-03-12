#include "driftsource.h"

Driftsource::~Driftsource()
{
}

double Driftsource::nextValue()
{
	const double driftLimit = -0.999999;

	double n = next();

	// limit drift to values > -1, so the time can't go back
	if (n < driftLimit)
		return driftLimit;

	return n;
}

ConstantDrift::ConstantDrift(double drift)
	: drift(drift)
{
}

double ConstantDrift::next()
{
	return drift;
}

BoundedDrift::BoundedDrift(const cPar& distribution)
	: distribution(distribution)
{
}

double BoundedDrift::next()
{
	return distribution.doubleValue();
}

BoundedDriftVariation::BoundedDriftVariation(const cPar& distribution, double max_drift_variation, double start_value, const simtime_t& tint)
	: BoundedDrift(distribution)
	, max_drift_change(tint.dbl() * max_drift_variation)
	, last_drift(start_value)
{
}

double BoundedDriftVariation::next()
{
	double drift = BoundedDrift::next();

	double diff = drift - last_drift;

	// limit the drift
	     if (diff >  max_drift_change)
		drift = last_drift + max_drift_change;
	else if (diff < -max_drift_change)
		drift = last_drift - max_drift_change;

	last_drift = drift;
	return drift;
}
