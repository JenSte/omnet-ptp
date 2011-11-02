#ifndef HARDWARECLOCK_H
#define HARDWARECLOCK_H

#include <omnetpp.h>
#include <queue>

class StorageWindow;

/// Implementation of a hardware (real, non perfect) clock.
///
/// This class implements a hardware clock according to the
/// paper "Accurate Clock Models for Simulating Wireless Sensor
/// Networks" by Federico Ferrari, Andreas Meier and Lothar Thiele.
class HardwareClock: public cSimpleModule
{
public:
	/// Class to hold all properties of a simulated hardware clock.
	class Properties {
		simtime_t _tint;

		/// length of update interval (in # of tints)
		size_t _u;

		/// storage window size (in # of tints)
		size_t _s;

	public:
		Properties();

		/// \returns	The time between two hold points of the
		///		appproximated time function.
		const simtime_t& tint() const { return _tint; }

		/// \returns	The length of the update interval
		///		(measured in number of times of tint).
		size_t u() const { return _u; }

		/// \returns	The size of the used storage window
		///		(measured in number of times of tint).
		size_t s() const { return _s; }

		/// \returns	The time between two updates of the storage window.
		simtime_t updateInterval() const { return _tint * _u; }

		/// Sets the values that are held in the object.
		///
		/// s is set to twice the value of u.
		///
		/// \param tint	The new value for the time between two hold points.
		/// \param u	The new value for the length of the update interval.
		void set(const simtime_t& tint, size_t u);
	};

private:
	Properties properties;

	StorageWindow* storageWindow;

	cMessage* selfMsg;

	void nextUpdate(cMessage* msg);

	/// Cleans up dynamically allocated resources
	/// between different simulation runs.
	void cleanup();

	/// Updates the text shown to the user in the GUI.
	void updateDisplay();

protected:
	/// Initializes the module.
	virtual void initialize();

	/// Handles the given message.
	virtual void handleMessage(cMessage *msg);

public:
	HardwareClock();
	~HardwareClock();
};

Define_Module(HardwareClock);

#endif
