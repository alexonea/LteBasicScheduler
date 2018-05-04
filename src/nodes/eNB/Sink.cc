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
    _statsUpdateCycle = par("statsTimeUnit");

    cMessage *updateNotification = new cMessage("update");
    scheduleAt(simTime() + _statsUpdateCycle, updateNotification);
}

void Sink::handleMessage(cMessage *msg)
{
    static unsigned int unitsElapsed = 0;
    if (msg->isSelfMessage())
    {
        unitsElapsed++;
        for (int i = 0; i < _numUsers; i++)
        {
            _userStats[i].avrgDatarate = (_userStats[i].avrgDatarate * (unitsElapsed / (unitsElapsed + 1))) + (_userStats[i].RBsSinceLastTimeUnit / (unitsElapsed + 1));
            _userStats[i].maxDatarate = (_userStats[i].maxDatarate < _userStats[i].RBsSinceLastTimeUnit) ? _userStats[i].RBsSinceLastTimeUnit : _userStats[i].maxDatarate;
            _userStats[i].minDatarate = (_userStats[i].minDatarate > _userStats[i].RBsSinceLastTimeUnit) ? _userStats[i].RBsSinceLastTimeUnit : _userStats[i].minDatarate;
            _userStats[i].RBsSinceLastTimeUnit = 0;
        }
    }
    else if (msg->arrivedOn("drain"))
    {
        ResourceBlock *rb = static_cast <ResourceBlock *> (msg);
        unsigned int userId = rb->getArrivalGate()->getIndex();

        unsigned int delay = simTime().inUnit(SIMTIME_S) - _userStats[userId].lastRBTimestamp;
        _userStats[userId].lastRBTimestamp = simTime().inUnit(SIMTIME_S);

        _userStats[userId].avrgDelay = (_userStats[userId].avrgDelay * (_userStats[userId].totalRBs / (_userStats[userId].totalRBs + 1))) + (delay / (_userStats[userId].totalRBs + 1));
        _userStats[userId].maxDelay = (_userStats[userId].maxDelay < delay) ? delay : _userStats[userId].maxDelay;
        _userStats[userId].minDelay = (_userStats[userId].minDelay < delay) ? delay : _userStats[userId].minDelay;

        _userStats[userId].totalRBs ++;
        _userStats[userId].RBsSinceLastTimeUnit ++;

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
}
