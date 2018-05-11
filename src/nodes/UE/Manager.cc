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

#include "Manager.h"

#include "../../messages/ResourceAllocation_m.h"
#include "../../messages/QueueControl_m.h"

Define_Module(Manager);

int Manager::count = 0;

void Manager::initialize()
{
    cModule *queueModule = this->getParentModule()->getSubmodule("queue");
    this->_id = count++;
    this->_queueManager = check_and_cast <Queue *> (queueModule);
}

void Manager::handleMessage(cMessage *msg)
{
    if (msg->arrivedOn("ctrl$i"))
    {
        // message from eNodeB
        if (strcmp(msg->getName(), "scheduler") == 0)
        {
            ResourceAllocation *ctrl = static_cast <ResourceAllocation *> (msg);
            int allowance = ctrl->getNumRBsToSend();

            /* Try communication using direct module calls */
            if (_queueManager != nullptr)
            {
                _queueManager->commandDequeue(allowance);
            }
            /* in case of error, fall back to the default message communication */
            else
            {
                QueueControl *req = new QueueControl("dequeue");
                req->setDequeue(allowance);
                send(req, "queueRequest");
            }

            EV << "User " << _id << " allowed to transmit " << allowance << " RBs in current scheduling cycle\n";
            delete ctrl;
        }
    }
    else
    {
        delete msg;
    }
}

long int Manager::commandReadUserQueueLength()
{
    Enter_Method_Silent();

    if (_queueManager != nullptr)
        return _queueManager->commandReadQueueLength();
    else
        return USER_QUEUE_LENGTH_NA;
}
