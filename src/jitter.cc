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
