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
    this->_id = count++;
}

void Manager::handleMessage(cMessage *msg)
{
    if (msg->arrivedOn("ctrl$i"))
    {
        // message from eNodeB
        if (strcmp(msg->getName(), "scheduler") == 0)
        {
            ResourceAllocation *ctrl = static_cast <ResourceAllocation *> (msg);
            QueueControl *req = new QueueControl("dequeue");
            int allowance = ctrl->getNumRBsToSend();

            req->setDequeue(allowance);
            EV << "User " << _id << " allowed to transmit " << allowance << " RBs in current scheduling cycle\n";

            send(req, "queueRequest");
            delete ctrl;
        }
    }
    else
    {
        delete msg;
    }
}
