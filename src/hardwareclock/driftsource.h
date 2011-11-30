#ifndef DRIFTSOURCE_H
#define DRIFTSOURCE_H

#include <omnetpp.h>

/// Abstract base class for drift value sources.
class Driftsource
{
protected:
	virtual double next() = 0;

public:
	virtual ~Driftsource();

	/// \returns	The next drift value.
	double nextValue();
};

/// \brief A constant drift source.
///
/// This source always returns the same constant drift.
class ConstantDrift: public Driftsource
{
	double drift;

protected:
	double next();

public:
	/// Initalizes the object.
	///
	/// \param drift	The constant drift value.
	ConstantDrift(double drift);
};

/// \brief Source for drift values within certain bounds.
class BoundedDrift: public Driftsource
{
	const cPar& distribution;

protected:
	double next();

public:
	/// Initalizes the object.
	///
	/// \param distribution	A OMNeT++ module parameter that evaluates to a distribution.
	BoundedDrift(const cPar& distribution);
};

/// \brief Source for drift values within bounds that are limited in their variation.
class BoundedDriftVariation: public BoundedDrift
{
	/// Maximum value that the drift can change between
	/// two values (between tint).
	double max_drift_change;

	double last_drift;

protected:
	double next();

public:
	/// Initalizes the object.
	///
	/// After evaluation the 'distribution' parameter, the resulting
	/// value is limited to the product of 'max_drift_variation' and 'tint'.
	///
	/// \param distribution	A OMNeT++ module parameter that evaluates to a distribution.
	/// \param max_drift_variation	The maximum value for \f$\frac{d}{dt}drift\f$.
	/// \param tint	Simulation time between two drift values.
	BoundedDriftVariation(const cPar& distribution, double max_drift_variation, const simtime_t& tint);
};

#endif
