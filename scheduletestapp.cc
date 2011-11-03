#include "scheduletestapp.h"
#include "hardwareclock.h"

void ScheduleTestApp::initialize()
{
	clocks.clear();

	//
	// find the hardware clocks
	//

	cModule* parent = getParentModule();
	if (!parent) {
		EV << "no parent module";
		return;
	}

	for (cModule::SubmoduleIterator i(parent); !i.end(); i++) {
		cModule* child = i();

		if (0 == strcmp("HardwareClock", child->getClassName()))
			clocks.push_back(check_and_cast<HardwareClock*>(child));
	}

	EV << clocks.size() << " clock(s) found\n";

	scheduleAt(SimTime::parse("10s"), new cMessage("schedule test"));
}

void ScheduleTestApp::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage()) {
		EV << "scheduling messages at hardware times\n";

		std::vector<HardwareClock*>::iterator it = clocks.begin();
		while (it != clocks.end()) {

			EV << "\n------------------\n";
			EV << (*it)->getName() << "'\n";
			EV << "------------------\n";
//			EV << "hardware time: " << (*it)->getHWtime() << '\n';

			simtime_t thenHardware = SimTime::parse("10.0255s");
			simtime_t thenReal;

			if ((*it)->HWtoSimTime(thenHardware, thenReal))
				EV << "HW2Sim: 10.0255s = " << thenReal << '\n';

			it++;
		}

	}

}
