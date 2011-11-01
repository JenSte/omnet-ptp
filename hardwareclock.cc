#include "hardwareclock.h"
#include "storagewindow.h"
#include "driftsource.h"

HardwareClock::Properties::Properties()
	: _u(0)
	, _s(0)
{
}

void HardwareClock::Properties::set(const simtime_t& tint, size_t u)
{
	// minimum values
	simtime_t tint_min = SimTime::parse("1ms");
	size_t u_min = 10;

	if (tint < tint_min) {
		EV << "warning: 'interval' value too small\n";
		_tint = tint_min;
	} else
		_tint = tint;

	if (u < u_min) {
		EV << "warning: 'update' value too small\n";
		_u = u_min;
	} else
		_u = u;

	_s = 2 * _u;
}

HardwareClock::HardwareClock()
	: storageWindow(NULL)
	, selfMsg(NULL)
{
}

HardwareClock::~HardwareClock()
{
	cancelAndDelete(selfMsg);

	cleanup();

}

void HardwareClock::nextUpdate(cMessage* msg)
{
	scheduleAt(simTime() + properties.updateInterval(), msg);
}

void HardwareClock::cleanup()
{
	if (storageWindow) {
		delete storageWindow;
		storageWindow = NULL;
	}

	// NOTE: selfMsg isn't deleted
}

void HardwareClock::initialize()
{
	// if needed, clean up stuff from the last run
	cleanup();

	properties.set(par("interval"), par("update"));

	EV << "update interval: " << properties.updateInterval() << "s\n";

	Driftsource* d = NULL;

	if (hasPar("drift_distribution")) {
		d = new BoundedDrift(par("drift_distribution"));
	} else
		d = new ConstantDrift(par("constant_drift"));

	storageWindow = new StorageWindow(properties, d);

	cMessage *msg = new cMessage("storage window update");
	nextUpdate(msg);
}

void HardwareClock::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage()) {
		// the only self message is to update the storage window

		storageWindow->update();
EV << "time after update: " << storageWindow->tBegin() << '\n';
		nextUpdate(msg);
	}
}
