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

#include "Queue.h"

#include <stdio.h>

#include "../../messages/QueueControl_m.h"

Define_Module(Queue);

void Queue::initialize()
{
    this->_config = ConfiguratorInterface::commandGetConfiguratorInstance(this);
}

void Queue::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        delete msg;
    }
    else if (msg->arrivedOn("request"))
    {
        QueueControl *req = static_cast <QueueControl *> (msg);
        int toDequeue = req->getDequeue();
        std::vector<int> allocation = req->getAllocation();

        this->commandDequeue(toDequeue, allocation);

        delete req;
    }
    else if (msg->arrivedOn("in"))
    {
        ResourceBlock *rb = static_cast <ResourceBlock *> (msg);
        this->commandQueue(&rb);
    }
    else
    {
        delete msg;
    }
}

void Queue::finish()
{
    while (!_queueData.empty())
    {
        ResourceBlock *rb = _queueData.front();
        _queueData.pop();

        delete rb;
    }
}

int Queue::commandReadQueueLength()
{
    Enter_Method("Queue::commandReadQueueLength");

    return _queueData.size();
}

int Queue::commandDequeue(int numItems, std::vector<int> allocation)
{
    Enter_Method("Queue::commandDequeue");

    int count = 0;
    while (!_queueData.empty() && count < numItems)
    {
        ResourceBlock *rb = _queueData.front();
        _queueData.pop();

        rb->setResourceGridId(allocation[count]);
        rb->setChannelQuality(par("channelQuality"));

        drop(rb);

        if (_config != nullptr)
        {
            sendDirect(rb, _config->commandGetENBUplinkEndpoint(rb->getSenderId()));
        }
        else
        {
            /*
             * We need to first create the connection and then send.
             * For now, error
             */

            error("Connection not established, cannot send");
            /* send(rb, "out"); */
        }

        count++;
    }

    return count;
}

int Queue::commandQueue(ResourceBlock **RBs, int numItems)
{
    Enter_Method("Queue::commandQueue");

    if (_queueData.size() > 1000)
        return 0;

    for (int i = 0; i < numItems; i++)
    {
        take(RBs[i]);
        _queueData.push(RBs[i]);
    }

    return numItems;
}
