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

#include <iomanip>

#include "../../messages/ResourceAllocation_m.h"
#include "RoundRobinSchedulingScheme.h"
#include "ProportionalFairSchedulingScheme.h"

Define_Module(Scheduler);

void Scheduler::initialize()
{
    this->_numRBs = 30;
    this->_schedCycle = par("schedCycle");
    this->_numConnections = par("size");

    /* this->_schedulingScheme = new RoundRobinSchedulingScheme(_numRBs, 7); */
    this->_schedulingScheme = new ProportionalFairSchedulingScheme(_numRBs);
    this->_userInfo = new UserInfo[_numConnections]();
    this->_userManager = new UserInfoInterface*[_numConnections];
    this->_signalUserAllocation = new simsignal_t[_numConnections]();

    this->_channelQuality = new double*[_numConnections];

    for (int i = 0; i < _numConnections; i++)
    {
        char tmp[64];

        /* Get user info interface instance for each user */
        sprintf(tmp, "BasicNetwork.n[%d].manager", i);
        cModule *userQueueManager = this->getModuleByPath(tmp);
        _userManager[i] = check_and_cast <UserInfoInterface*> (userQueueManager);

        /* Register allocation signal for each user */
        sprintf(tmp, "user%d-allocation", i);
        _signalUserAllocation[i] = registerSignal(tmp);

        cProperty *statisticTemplate = this->getProperties()->get("statisticTemplate", "user-allocation");
        getEnvir()->addResultRecorders(this, _signalUserAllocation[i], tmp, statisticTemplate);

        this->_channelQuality[i] = new double[_numRBs]();
        for (int j = 0; j < _numRBs; j++)
            _channelQuality[i][j] = 0.5;

        this->_userInfo[i].channelQuality = _channelQuality[i];
    }

    cMessage *notification = new cMessage("scheduler");
    scheduleAt(simTime() + _schedCycle, notification);
}

void Scheduler::commandUpdateChannelQuality(int userId, int RB, double value)
{
    if (RB < 0 || RB >= _numRBs)
        return;

    if (userId < 0 || userId >= _numConnections)
        return;

    _channelQuality[userId][RB] = value;
}

void Scheduler::_readUserInfo()
{
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
}

void Scheduler::_printChannelQuality()
{
    std::stringstream out;
    out << std::setw(7) << std::left << "Usr/RB";
    for (int i = 0; i < _numRBs; i++)
        out << std::setw(5) << std::left << i;

    for (int i = 0; i < _numConnections; i++)
    {
        out << std::endl << std::setw(7) << std::left << i;
        for (int j = 0; j < _numRBs; j++)
        {
            out << std::setw(5) << std::left << std::setprecision(2) <<_channelQuality[i][j];
        }
    }

    EV << out.str() << std::endl;
}

void Scheduler::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        this->_readUserInfo();
        this->_printChannelQuality();

        SchedulingDecision *decision = _schedulingScheme->schedule(_numConnections, _userInfo);
        if (decision != nullptr)
        {
            for (int i = 0; i < _numConnections; i++)
            {
                SchUserAllocation userAllocation = decision->getAllocationForUser(i);
                if (userAllocation.count != 0)
                {
                    std::vector<int> gridAllocation;
                    ResourceAllocation *ctrl = new ResourceAllocation("scheduler");

                    EV << "Allocation for user " << i << ": ";
                    for (int j = 0; j < userAllocation.count; j++)
                    {
                        EV << userAllocation.RBs[j].RB << " ";
                        gridAllocation.push_back(userAllocation.RBs[j].RB);
                    }
                    EV << endl;

                    ctrl->setNumRBsToSend(userAllocation.count);
                    ctrl->setGridAllocation(gridAllocation);

                    send(ctrl, this->gate("ctrl$o", i));
                }

                emit(_signalUserAllocation[i], (unsigned long int) userAllocation.count);
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
