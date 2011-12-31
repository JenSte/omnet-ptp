#ifndef SOFTWARECLOCK_H
#define SOFTWARECLOCK_H

#include <omnetpp.h>

class HardwareClock;

/// Module to add a correction factor to a hardware clock.
///
/// This module uses the first found HardwareClock module
/// in the same compound module and applies a correction
/// factor to the hardware timestamps.
class SoftwareClock: public cSimpleModule
{
	HardwareClock* clock;

	// factor to correct the hardware time
	double factor;

	// limit for the correction factor
	double factorLimit;

	/// Vector to record the deviation between the software
	/// and simulation time.
	cOutVector deviationVector;

	/// Vector to record the factor.
	cOutVector factorVector;

protected:
	/// Initializes the module.
	virtual void initialize();

public:
	/// \returns	The hardware time corrected by the factor (= software time).
	SimTime getSWtime();

	/// Sets the correction factor.
	/// \param f	The new value for the correction factor.
	void setFactor(double f);

	/// \returns	A pointer to the first found software clock in the compound module 'parent',
	//		throws a std::logic_error if there is no hardware clock.
	static SoftwareClock* findFirstClock(const cModule* parent);
};

#endif
