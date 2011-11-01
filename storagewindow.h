#ifndef STORAGEWINDOW_H
#define STORAGEWINDOW_H

class Driftsource;
class Hardwareclock {
	class Properties;
};

class StorageWindow
{
	struct holdPoint
	{
		simtime_t hardwareTime;
		double drift;
	};

	std::vector<holdPoint> data;

	const HardwareClock::Properties& properties;

	Driftsource* source;

	void fillRange(std::vector<holdPoint>::iterator first, std::vector<holdPoint>::iterator last);

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
	simtime_t tBegin() const { return data[0].hardwareTime; }

	/// Returns the time at the end of the storage window.
	simtime_t tEnd() const { return data[data.size() - 1].hardwareTime + properties.tint(); }
};

#endif
