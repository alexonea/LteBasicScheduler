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

#ifndef __LTEBASICSCHEDULER_QUEUE_H_
#define __LTEBASICSCHEDULER_QUEUE_H_

#include <omnetpp.h>
#include <queue>

#include "../configurator/ConfiguratorInterface.h"
#include "../../messages/ResourceBlock_m.h"

using namespace omnetpp;

class Queue : public cSimpleModule
{
public:
    int commandReadQueueLength();
    int commandDequeue(int numItems, std::vector<int> allocation);
    int commandQueue(ResourceBlock **RBs, int numItems = 1);
private:
    std::queue<ResourceBlock *> _queueData;
    ConfiguratorInterface *_config;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

#endif
