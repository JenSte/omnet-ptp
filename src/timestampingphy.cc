#include "timestampingphy.h"

Define_Module(TimestampingPhy);

void TimestampingPhy::initialize()
{
	ext_i = gate("external$i");
	ext_o = gate("external$o");
	int_o = gate("internal$o");
}

void TimestampingPhy::handleMessage(cMessage *msg)
{
	if (ext_i == msg->getArrivalGate()) {
		send(msg, int_o);
	} else { // message arrived at internal$i
		send(msg, ext_o);
	}
}
