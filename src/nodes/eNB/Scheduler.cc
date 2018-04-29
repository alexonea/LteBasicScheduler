//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Scheduler.h"

Define_Module(Scheduler);

void Scheduler::initialize()
{
    this->_schedCycle = par("schedCycle");
    this->_numConnections = par("size");

    cMessage *notification = new cMessage("scheduler");
    scheduleAt(simTime() + _schedCycle, notification);
}

void Scheduler::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        // invoke scheduler here
        for (int i = 0; i < _numConnections; i++)
        {
            cMessage *ctrl = new cMessage("allow");
            send(ctrl, this->gate("ctrl$o", i));
        }

        scheduleAt(simTime() + _schedCycle, msg);
    }
}
