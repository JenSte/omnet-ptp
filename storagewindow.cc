#include "driftsource.h"
#include "hardwareclock.h"
#include "storagewindow.h"

StorageWindow::StorageWindow(const HardwareClock::Properties& properties, Driftsource* source)
	: properties(properties)
	, source(source)
{
	data.resize(properties.s());

	std::vector<holdPoint>::iterator it = data.begin();

	it->hardwareTime = simTime();
	it->drift = source->nextValue();

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

		first->hardwareTime = pre->hardwareTime + properties.tint() * (1 + pre->drift);
		first->drift = source->nextValue();

		first++;
	}
}
