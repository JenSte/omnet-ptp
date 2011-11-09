#ifndef HARDWARECLOCK_H
#define HARDWARECLOCK_H

#include <omnetpp.h>
#include <queue>

class StorageWindow;

/// \brief Implementation of a hardware (real, non perfect) clock.
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
	struct QueuedMessage
	{
		simtime_t time;
		cMessage* msg;
		cSimpleModule* self;

	public:
		QueuedMessage(const simtime_t& time, cMessage* msg, cSimpleModule* self)
			: time(time)
			, msg(msg)
			, self(self)
		{
		}

		bool operator<(const QueuedMessage& rhs) const
		{
			/// because we want to have the element with the *smallest*
			/// timestamp to be the one with the highest priority, we
			/// swap > and <
			return time > rhs.time;
		}
	};

	/// Queued messages that can't be scheduled yet.
	std::priority_queue<QueuedMessage> queue;

	/// The properties of this clock.
	Properties properties;

	StorageWindow* storageWindow;

	/// Message to schedule storage window updates.
	cMessage* selfMsg;

	/// Schedules the next update of the storage window.
	///
	/// \param msg	The message used as a self message.
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
	/// Initializes the hardware clock.
	HardwareClock();

	/// Cleans up the used resources of the hardware clock.
	~HardwareClock();

	/// Converts a hardware timestamp to a simulation timestamp.
	///
	/// \param hwtime	A hardware timestamp that is in the storage window
	///			of the clock.
	/// \param [out] realtime	The timestamp where the output is placed.
	/// \returns	true if the hardware timestamp could be converted, false
	///		otherwise (realtime is then undefined).
	bool HWtoSimTime(const simtime_t& hwtime, simtime_t& realtime) const;

	/// \returns	The hardware time of the clock that corresponds to the
	///		current simulation time.
	simtime_t getHWtime() const;

	/// Schedules a message at hardware time.
	///
	/// Note that the message doesn't have to be scheduled immediately, if the
	/// timestamp lies outside of the current precalculated time range of the clock,
	/// it's stored and scheduled at a later time (Take this in account if you
	/// want to cancel the message).
	///
	/// \param time	The hardware time when the message should be scheduled.
	/// \param msg	The message to schedule.
	/// \param self	The object that is scheduling the message.
	void scheduleAtHWtime(const simtime_t& time, cMessage* msg, cSimpleModule* self);
};

#endif
