#ifndef STORAGEWINDOW_H
#define STORAGEWINDOW_H

#include <omnetpp.h>
#include "hardwareclock.h"

class Driftsource;

class StorageWindow
{
public:
	struct holdPoint
	{
		simtime_t realTime;
		simtime_t hardwareTime;
		double drift;
	};

private:
	std::vector<holdPoint> data;

	const HardwareClock::Properties& properties;

	Driftsource* source;

	cOutVector driftVector;
	cOutVector timeVector;
	cOutVector deviationVector;

	simtime_t _hardwareTimeEnd;

	void fillRange(std::vector<holdPoint>::iterator first, std::vector<holdPoint>::iterator last);

	void recordVectors(const simtime_t& realTime, const simtime_t& hardwareTime, double drift);

public:
	/// Initializes the storage window.
	///
	/// \param properties	Properties object of the simulated hardware clock, used
	///			to determine things like the length of the storage window, etc.
	/// \param source	Pointer to a source of drift values, the StorageWindow object
	///			takes ownership of the object being passed.
	StorageWindow(const HardwareClock::Properties& properties, Driftsource* source);

	~StorageWindow();

	/// Updates the storage window.
	///
	/// The first u values in the storge window are discarded, the
	/// remaining ones are moved to the front and the empty space
	/// is filled up with new values.
	void update();

	/// Returns the time at the beginning of the storage window.
	const simtime_t& hardwareTimeBegin() const { return data[0].hardwareTime; }

	/// Returns the time at the end of the storage window.
	const simtime_t& hardwareTimeEnd() const { return _hardwareTimeEnd; }

	/// Returns the hold point at index idx.
	///
	/// If the index is out of bounds, an std::logic_error exception is thrown.
	/// \param idx	The index of the requested hold point.
	/// \returns	The hold point at index idx.
	const StorageWindow::holdPoint& at(size_t idx) const;

	/// Calculates the hold point index for a timestamp.
	///
	/// \param	A simulation timestamp.
	/// \returns	The index of the hold point in what the simulation time lies.
	size_t indexOf(const simtime_t& t) const;
};

#endif
