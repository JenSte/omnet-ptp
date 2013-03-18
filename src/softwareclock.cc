/*
    Clock Synchronization Simulation in OMNet++
    Copyright (C) 2012 Jens Steinhauser

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

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
