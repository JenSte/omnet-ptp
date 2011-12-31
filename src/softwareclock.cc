#include "softwareclock.h"
#include "hardwareclock.h"
#include <exception>

Define_Module(SoftwareClock);

void SoftwareClock::initialize()
{
	clock = HardwareClock::findFirstClock(getParentModule());
	EV << "using hardware clock with id " << clock->getId() << '\n';

	factor = 0.0;

	factorLimit = 0.05;

	WATCH_RW(factor);

	deviationVector.setName("time_deviation");
	deviationVector.setUnit("s");

	factorVector.setName("factor");
}

SimTime SoftwareClock::getSWtime()
{
	SimTime swt = clock->getHWtime() + factor;

	deviationVector.record(swt - simTime());

	return swt;
}

void SoftwareClock::setFactor(double f)
{
	     if (f >  factorLimit)
		f =  factorLimit;
	else if (f < -factorLimit)
		f = -factorLimit;

	factor = f;

	factorVector.record(f);
}

SoftwareClock* SoftwareClock::findFirstClock(const cModule* parent)
{
	for (cModule::SubmoduleIterator i(parent); !i.end(); i++) {
		cModule* child = i();

		if (0 == strcmp("SoftwareClock", child->getClassName()))
			return check_and_cast<SoftwareClock*>(child);
	}

	throw std::logic_error("no SoftwareClock found");
}
