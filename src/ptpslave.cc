#include "ptpslave.h"
#include "ptp.h"
#include "ptppacket_m.h"
#include "softwareclock.h"
#include <Ieee802Ctrl_m.h>
#include <sstream>

Define_Module(PtpSlave);

PtpSlave::PtpSlave()
	: msDelay("master_slave_delay", 100)
	, smDelay("slave_master_delay", 100)
{
	controller.r[0] =  1;
	controller.r[1] = 12.5;
	controller.r[2] = 25;
}

void PtpSlave::initialize()
{
	clock = SoftwareClock::findFirstClock(getParentModule());
	EV << "using software clock with id " << clock->getId() << '\n';

	for (int i = 0; i < CONTROLLER_STAGES; i++) {
		std::stringstream sse;
		std::stringstream sso;

		sse << "e " << i;
		sso << "o " << i;

		errorVector[i].setName(sse.str().c_str());
		errorVector[i].setUnit("s");

		offsetVector[i].setName(sso.str().c_str());
		offsetVector[i].setUnit("s");
	}

	controllerEnabled = par("controllerEnabled");
	memset(controller.e, 0, sizeof(controller.e));
	controller.last = SimTime();

	// this assumes that the packet delays are
	// between 1 and 100 micro seconds
	msDelay.setRange(1e-6, 100e-6);
	smDelay.setRange(1e-6, 100e-6);

	msDelayVector.setName("master_slave_delay");
	msDelayVector.setUnit("s");
	smDelayVector.setName("slave_master_delay");
	smDelayVector.setUnit("s");

	msDelayRealVector.setName("real master_slave_delay");
	msDelayRealVector.setUnit("s");
	smDelayRealVector.setName("real slave_master_delay");
	smDelayRealVector.setUnit("s");
}

void PtpSlave::finish()
{
	msDelay.recordAs("master_slave_delay", "s");
	smDelay.recordAs("slave_master_delay", "s");
}

void PtpSlave::sendDelayReq(const MACAddress& masterMAC)
{
	PtpPacket* p = Ptp::newDelayReqPacket(masterMAC);

	// gets overwritten by the phy when hardware
	// timestamping is used
	p->setTtx(clock->getSWtime());

	send(p, "port$o");
}

void PtpSlave::correct()
{
//	double offset = ( (timestamps.t[1] - timestamps.t[0])
//	                - (timestamps.t[3] - timestamps.t[2]) ).dbl() / 2.0;
//
	double offset = ( timestamps.msDelay - timestamps.smDelay ).dbl() / 2.0;

	if (!controllerEnabled)
		return;

	//
	// controller
	//

	double Ta = (simTime() - controller.last).dbl();
	controller.last = simTime();
//	double Ta = 0.1;

	// offset sum
	double os = 0.0;

	for (int i = CONTROLLER_STAGES - 1; i >= 0; i--) {
		if (0 == i)
			controller.e[0]  = -offset;
		else
			controller.e[i] += controller.e[i-1] * Ta;

		errorVector[i].record(controller.e[i]);

		double o = controller.e[i] * controller.r[i];
		offsetVector[i].record(o);

		os += o;
	}

	clock->setFactor(os);
}

void PtpSlave::handleMessage(cMessage* msg)
{
	if (PtpPacket* ptp = dynamic_cast<PtpPacket*>(msg)) {
		switch (ptp->getType()) {
		case Ptp::Sync:
			// if there is no hardware timestamp by
			// the phy, use a software timestamp
			if (0 == ptp->getTrx().raw())
				ptp->setTrx(clock->getSWtime());

			// if this is the first sync packet received
			// from a master clock, use it from now on
			if (masterMAC.isUnspecified()) {
				Ieee802Ctrl* c = check_and_cast<Ieee802Ctrl*>(ptp->getControlInfo());

				masterMAC = c->getSrc();
			}

			sendDelayReq(masterMAC);

			timestamps.msDelay = ptp->getTrx() - ptp->getTtx();
			msDelay.collect(timestamps.msDelay);
			msDelayVector.record(timestamps.msDelay);

			msDelayRealVector.record(ptp->getRealTrx() - ptp->getRealTtx());

			break;

		case Ptp::Delay_Resp:
			timestamps.smDelay = ptp->getTrx() - ptp->getTtx();
			smDelay.collect(timestamps.smDelay);
			smDelayVector.record(timestamps.smDelay);

			smDelayRealVector.record(ptp->getRealTrx() - ptp->getRealTtx());

			correct();

			break;

		default:
			break;
		}
	}

	delete msg;
}
