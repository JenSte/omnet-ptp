#include "ptpmaster.h"
#include <Ieee802Ctrl_m.h>

Define_Module(PtpMaster);

void PtpMaster::initialize()
{
	Ieee802Ctrl* ctrl = new Ieee802Ctrl();


}

void PtpMaster::handleMessage(cMessage* msg)
{


}
