#include "scheduletestapp.h"
#include "hardwareclock.h"

int ScheduleTestApp::numInitStages() const
{
	return 2;
}

void ScheduleTestApp::initialize(int stage)
{
	simtime_t times[] = { SimTime::parse("55.503ms"), SimTime::parse("10.3s") };

	if (1 == stage) {
		// the initialization is done in
		// stage 1, because in stage 0 the
		// clocks aren't ready yet

		pending = 0;

		//
		// find the hardware clocks
		//
		cModule* parent = getParentModule();
		if (!parent) {
			EV << "no parent module";
			return;
		}

		size_t cnt = 0;
		for (cModule::SubmoduleIterator i(parent); !i.end(); i++) {
			cModule* child = i();

			if (0 != strcmp("HardwareClock", child->getClassName())) {
				// not a clock
				continue;
			}

			HardwareClock* clock = check_and_cast<HardwareClock*>(child);
			cnt++;

			//
			// schedule some self messages on every clock
			//
			for (size_t i = 0; i < sizeof(times) / sizeof(simtime_t); i++) {
				ScheduleTestMsg* msg = new ScheduleTestMsg();
				msg->setName("ScheduleTestMsg");
				msg->clock = clock;
				msg->time = times[i];

				clock->scheduleAtHWtime(times[i], msg, this);
				pending++;
			}
		}

		EV << cnt << " clock(s) found, " << pending << " messages scheduled \n";
	}
}

std::string extend(std::string s)
{
	s.resize(17, ' ');
	return s;
}

void ScheduleTestApp::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage()) {
		ScheduleTestMsg* m = check_and_cast<ScheduleTestMsg*>(msg);

		pending--;

		EV << "HW time scheduled message received from clock '" << m->clock->getName() << "'\n";
		EV << "requested arrival time: " << extend(m->time.str())
			<< "sec\n";
		EV << "HW time of the clock:   " << extend(m->clock->getHWtime().str())
			<< "sec   (should be equal or very close to the above timestamp)\n";
		EV << "simulation time:        " << extend(simTime().str())
			<< "sec   (doesn't need to be equal depending on the clock drift)\n";
	}

}
void ScheduleTestApp::finish()
{
	if (0 == pending)
		EV << "TestApp: looks good, got all test messages\n";
	else
		EV << "TestApp: hey, " << pending << " test messages are still pending!\n";
}
