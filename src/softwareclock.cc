#include "softwareclock.h"
#include "hardwareclock.h"
#include <exception>

Define_Module(SoftwareClock);

void SoftwareClock::initialize()
{
	clock = HardwareClock::findFirstClock(getParentModule());
}

SimTime SoftwareClock::getSWtime() const
{
	return clock->getHWtime();
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
