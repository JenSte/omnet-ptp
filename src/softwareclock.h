#ifndef SOFTWARECLOCK_H
#define SOFTWARECLOCK_H

#include <omnetpp.h>

class HardwareClock;

class SoftwareClock: public cSimpleModule
{
	HardwareClock* clock;

	double factor;

	/// Vector to record the deviation between the software
	/// and simulation time.
	cOutVector deviationVector;

	/// Vector to record the factor.
	cOutVector factorVector;

protected:
	/// Initializes the module.
	virtual void initialize();

public:
	SimTime getSWtime();

	void setFactor(double f);

	/// \returns	A pointer to the first found software clock in the compound module 'parent',
	//		throws a std::logic_error if there is no hardware clock.
	static SoftwareClock* findFirstClock(const cModule* parent);
};

#endif
