#ifndef DRIFTSOURCE_H
#define DRIFTSOURCE_H

#include <omnetpp.h>

class Driftsource
{
public:
	virtual ~Driftsource();

	virtual double nextValue() = 0;
};

class ConstantDrift: public Driftsource
{
	double drift;

public:
	ConstantDrift(double drift);

	double nextValue();
};

class BoundedDrift: public Driftsource
{
	const cPar& distribution;

public:
	BoundedDrift(const cPar& distribution);

	double nextValue();
};

class BoundedDriftVariation: public BoundedDrift
{
	/// Maximum value that the drift can change between
	/// two values (between tint).
	double max_drift_change;

	double last_drift;

public:
	BoundedDriftVariation(const cPar& distribution, double max_drift_variation, const simtime_t& tint);

	double nextValue();
};

#endif
