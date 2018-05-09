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

        this->commandDequeue(toDequeue);

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

int Queue::getQueueLength()
{
    return _queueData.size();
}

int Queue::commandDequeue(int numItems)
{
    int itemsLeft = numItems;
    while (!_queueData.empty() && itemsLeft > 0)
    {
        ResourceBlock *rb = _queueData.front();
        _queueData.pop();
        send(rb, "out");

        itemsLeft--;
    }

    return numItems - itemsLeft;
}

int Queue::commandQueue(ResourceBlock **RBs, int numItems)
{
    for (int i = 0; i < numItems; i++)
    {
        _queueData.push(RBs[i]);
    }

    return numItems;
}
