#include "storagewindow.h"
#include "driftsource.h"

StorageWindow::StorageWindow(const HardwareClock::Properties& properties, Driftsource* source)
	: properties(properties)
	, source(source)
{
	data.resize(properties.s());

	std::vector<holdPoint>::iterator it = data.begin();

	driftVector.setName("drift");
	timeVector.setName("hardware_time");
	deviationVector.setName("time_deviation");

	timeVector.setUnit("s");
	deviationVector.setUnit("s");

	simtime_t now = simTime();

	it->realTime = now;
	it->hardwareTime = now;
	it->drift = source->nextValue();

	recordVectors(now, now, it->drift);

	fillRange(it + 1, data.end());
}

StorageWindow::~StorageWindow()
{
	delete source;
}

void StorageWindow::update()
{
	data = std::vector<holdPoint>(data.begin() + properties.u(), data.end());
	data.resize(properties.s());

	fillRange(data.begin() + (properties.s() - properties.u()), data.end());
}

void StorageWindow::fillRange(std::vector<holdPoint>::iterator first, std::vector<holdPoint>::iterator last)
{
	while (first != last) {
		std::vector<holdPoint>::iterator pre = first - 1;

		first->realTime = pre->realTime + properties.tint();
		first->hardwareTime = pre->hardwareTime + properties.tint() * (1 + pre->drift);
		first->drift = source->nextValue();

		recordVectors(first->realTime, first->hardwareTime, first->drift);

		first++;
	}
}

void StorageWindow::recordVectors(const simtime_t& realTime, const simtime_t& hardwareTime, double drift)
{
	driftVector.recordWithTimestamp(realTime, drift);
	timeVector.recordWithTimestamp(realTime, hardwareTime);
	deviationVector.recordWithTimestamp(realTime, hardwareTime - realTime);
}
