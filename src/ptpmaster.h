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

#ifndef PTPMASTER
#define PTPMASTER

#include <omnetpp.h>

class PtpPacket;

class PtpMaster: public cSimpleModule
{
	SimTime syncInterval;

	cMessage* selfMsg;

	void sendSync();

	void answerDelayReq(const PtpPacket* delayReq);

protected:
	void initialize();

	virtual void handleMessage(cMessage* msg);

public:
	PtpMaster();
};

#endif
