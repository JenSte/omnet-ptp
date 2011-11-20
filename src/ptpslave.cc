#include "ptpslave.h"

Define_Module(PtpSlave);

void PtpSlave::initialize()
{


}

void PtpSlave::handleMessage(cMessage* msg)
{
	delete msg;
}
