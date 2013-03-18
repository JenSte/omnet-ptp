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

#include "jitter.h"

Define_Module(Jitter);

void Jitter::initialize()
{
	// cache gate pointers
	u_i = gate("upperLayer$i");
	u_o = gate("upperLayer$o");
	l_o = gate("lowerLayer$o");

	// reset to zero
	last = SimTime();

	jitterHistogram.setName("jitter_distribution");
}

void Jitter::finish()
{
	jitterHistogram.recordAs("jitter_distribution");
}

void Jitter::handleMessage(cMessage* msg)
{
	if (u_i == msg->getArrivalGate()) {
		send(msg, l_o);
	} else {
		SimTime s = simTime() + par("delay");

		// make sure that we don't reorder the packets
		if (s < last)
			s = last + 2e-6;

		SimTime delay = s - simTime();

		last = s;

		sendDelayed(msg, delay, u_o);

		jitterHistogram.collect(delay);
	}
}
