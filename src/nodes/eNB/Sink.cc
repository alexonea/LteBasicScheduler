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

#include "Sink.h"

#include "../../messages/ResourceBlock_m.h"

Define_Module(Sink);

void Sink::initialize()
{
    _numUsers = par("size");
    _userStats = new UserStats[_numUsers]();

    _signalUserRBs = new simsignal_t[_numUsers]();
    _statsUpdateCycle = par("statsTimeUnit");

    for (int i = 0; i < _numUsers; i++)
    {
        char userIdStr[12];
        sprintf(userIdStr, "user%d-RBs", i);
        _signalUserRBs[i] = registerSignal(userIdStr);

        cProperty *statisticTemplate = this->getProperties()->get("statisticTemplate", "user-RBs");
        getEnvir()->addResultRecorders(this, _signalUserRBs[i], userIdStr, statisticTemplate);
    }

    _statsUpdateCycle = par("statsTimeUnit");

    cMessage *updateNotification = new cMessage("update");
    scheduleAt(simTime() + _statsUpdateCycle, updateNotification);
}

void Sink::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        for (int i = 0; i < _numUsers; i++)
        {
            _userStats[i].instDatarate = (double) _userStats[i].bitsSinceLastTimeUnit / _statsUpdateCycle;

            emit(_signalUserRBs[i], (unsigned long int) _userStats[i].instDatarate);

            _userStats[i].bitsSinceLastTimeUnit = 0;
        }

        scheduleAt(simTime() + _statsUpdateCycle, msg);
    }
    else if (msg->arrivedOn("drain"))
    {
        ResourceBlock *rb = static_cast <ResourceBlock *> (msg);
        unsigned int userId = rb->getArrivalGate()->getIndex();

        _userStats[userId].totalRBs ++;
        _userStats[userId].bitsSinceLastTimeUnit += rb->getSize();

        EV << "Received " << rb->getSize() << " bits from user " << userId << endl;
        EV << "Total number of bits in the last 1ms is now " << _userStats[userId].bitsSinceLastTimeUnit << endl;

        delete rb;
    }
    else
    {
        delete msg;
    }
}

void Sink::finish()
{
    delete [] _userStats;
    delete [] _signalUserRBs;
}
