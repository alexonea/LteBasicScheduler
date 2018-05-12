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

#include "../../messages/ResourceAllocation_m.h"
#include "RoundRobinSchedulingScheme.h"

Define_Module(Scheduler);

void Scheduler::initialize()
{
    this->_schedCycle = par("schedCycle");
    this->_numConnections = par("size");

    this->_schedulingScheme = new RoundRobinSchedulingScheme();
    this->_userInfo = new UserInfo[_numConnections]();
    this->_userManager = new UserInfoInterface*[_numConnections];

    for (int i = 0; i < _numConnections; i++)
    {
        char path[128];
        sprintf(path, "BasicNetwork.n[%d].manager", i);
        cModule *userQueueManager = this->getModuleByPath(path);
        _userManager[i] = check_and_cast <UserInfoInterface*> (userQueueManager);
    }

    cMessage *notification = new cMessage("scheduler");
    scheduleAt(simTime() + _schedCycle, notification);
}

void Scheduler::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        /* Read the user queue length */
        for (int i = 0; i < _numConnections; i++)
        {
            /* Try to read the user queue length, otherwise set it to N/A */
            if (_userManager[i] != nullptr)
            {
                _userInfo[i].queueLength = _userManager[i]->commandReadUserQueueLength();
            }
            else
            {
                _userInfo[i].queueLength = USER_QUEUE_LENGTH_NA;
            }

            EV << "Queue length for user " << i << ": " << _userInfo[i].queueLength << endl;
        }

        SchedulingDecision *decision = _schedulingScheme->schedule(_numConnections, _userInfo);
        if (decision != nullptr)
        {
            for (int i = 0; i < _numConnections; i++)
            {
                int toSend = decision->getAllocationForUser(i).count;
                if (toSend != 0)
                {
                    ResourceAllocation *ctrl = new ResourceAllocation("scheduler");
                    ctrl->setNumRBsToSend(toSend);

                    send(ctrl, this->gate("ctrl$o", i));
                }
            }

        }

        delete decision;
        scheduleAt(simTime() + _schedCycle, msg);
    }
}

void Scheduler::finish()
{
    delete _schedulingScheme;
}
