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

#include "RGen.h"

#include <cstdlib>
#include <ctime>

#include "../../messages/DataPacket_m.h"

Define_Module(RGen);

void RGen::initialize()
{
    cModule *transcieverModule = this->getParentModule()->getSubmodule("transciever");
    this->_defaultPacketSize = par("defaultPacketSize");
    this->_randomGen = this->getRNG(0);
    this->_nextEventOffset = _randomGen->doubleRand();

    this->_transcieverManager = check_and_cast <Transciever *> (transcieverModule);

    cMessage *notification = new cMessage("notification");
    scheduleAt(simTime() + _nextEventOffset, notification);
}

void RGen::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        DataPacket *dp = new DataPacket("data");
        dp->setSize(_defaultPacketSize);

        /*
         * We must also set the sender ID here, for now it is not set
         */

        /* try using direct call, otherwise use default message communication */
        if (_transcieverManager != nullptr)
        {
            int totalRBs;
            (void) _transcieverManager->commandEncode(dp, totalRBs);
            (void) totalRBs;
        }
        else
        {
            send(dp, "out");
        }

        this->_nextEventOffset = _randomGen->doubleRand();
        scheduleAt(simTime() + _nextEventOffset, msg);
    }
}
