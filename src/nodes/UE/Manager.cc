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

Define_Module(Manager);

void Manager::initialize()
{
    // TODO - Generated method body
}

void Manager::handleMessage(cMessage *msg)
{
    if (msg->arrivedOn("ctrl$i"))
    {
        // message from eNodeB
        if (strcmp(msg->getName(), "allow") == 0)
        {
            // allowance to transmit, so we transmit 1 RB
            cMessage *req = new cMessage("dequeue 1 RB");
            send(req, "queueRequest");

            delete msg;
        }
    }
}
