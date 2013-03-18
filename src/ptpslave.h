/*
    Clock Synchronization Simulation in OMNet++
    Copyright (C) 2012 Jens Steinhauser

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef PTPSLAVE
#define PTPSLAVE

#include <omnetpp.h>
#include <MACAddress.h>

#define CONTROLLER_STAGES 3

class SoftwareClock;

class PtpSlave: public cSimpleModule
{
	SoftwareClock* clock;

	MACAddress masterMAC;

	// the timestamps from the last received
	// sync (0/1) and delay_resp(2/3) packets
	struct Timestamps {
		SimTime msDelay;
		SimTime smDelay;
	} timestamps;

	bool controllerEnabled;

	struct {
		double e[CONTROLLER_STAGES];
		double r[CONTROLLER_STAGES];

		SimTime last;
	} controller;

	cOutVector errorVector[CONTROLLER_STAGES];
	cOutVector offsetVector[CONTROLLER_STAGES];

	cDoubleHistogram msDelay;
	cDoubleHistogram smDelay;

	cOutVector msDelayVector;
	cOutVector smDelayVector;

	cOutVector msDelayRealVector;
	cOutVector smDelayRealVector;

	void sendDelayReq(const MACAddress& masterMAC);

	void correct();

protected:
	void initialize();

	void finish();

	virtual void handleMessage(cMessage* msg);

public:
	PtpSlave();
};

#endif
