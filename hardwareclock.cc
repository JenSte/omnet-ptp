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
	updateDisplay();

	cMessage *msg = new cMessage("storage window update");
	nextUpdate(msg);
}

void HardwareClock::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage()) {
		// the only self message is to update the storage window

		storageWindow->update();
		updateDisplay();
EV << "time after update: " << storageWindow->tBegin() << '\n';
		nextUpdate(msg);
	}
}

void HardwareClock::updateDisplay()
{
	simtime_t real = simTime();
	simtime_t hard = storageWindow->tBegin();

	simtime_t diff = hard - real;
	double d = fabs(diff.dbl());

	const char  sign = diff < 0 ? '-' : '+';
	const char* unit;
	int val;

	       if (d > 1) {
		val = d;        unit =  "s";
	} else if (d > 1e-3) {
		val = d * 1e3;  unit = "ms";
	} else if (d > 1e-6) {
		val = d * 1e6;  unit = "us";
	} else if (d > 1e-9) {
		val = d * 1e9;  unit = "ns";
	} else if (d > 1e-12) {
		val = d * 1e12; unit = "ps";
	} else {
		getDisplayString().setTagArg("t", 0, "delta t: ca. 0s");
		return;
	}

	char buf[64];
	snprintf(buf, sizeof(buf), "delta t: ca. %c%i%s", sign, val, unit);

	getDisplayString().setTagArg("t", 0, buf);
}
