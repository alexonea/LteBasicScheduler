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

#include "Configurator.h"

Define_Module(Configurator);

void Configurator::initialize()
{
    this->_netSize = par("networkSize");

    this->_eNodeB = this->getParentModule()->getSubmodule("eNodeB");
    this->_users = new cModule*[_netSize]();

    for (int i = 0; i < _netSize; i++)
    {
        _users[i] = this->getParentModule()->getSubmodule("n", i);
    }
}

cGate* Configurator::commandGetENBControlEndpoint(int userId)
{
    if (_eNodeB == nullptr)
        return nullptr;

    return _eNodeB->gate("ctrl$i", userId);
}

cGate* Configurator::commandGetENBUplinkEndpoint(int userId)
{
    if (_eNodeB == nullptr)
        return nullptr;

    return _eNodeB->gate("ulink", userId);
}

cGate* Configurator::commandGetUserControlEndpoint(int userId)
{
    if (_users[userId] == nullptr)
        return nullptr;

    return _users[userId]->gate("ctrl$i");
}

cGate* Configurator::commandGetUserDownlinkEndpoint(int userId)
{
    if (_users[userId] == nullptr)
        return nullptr;

    /*
     * Not yet implemented! Only UL working for now
     */
    return nullptr;
}

void Configurator::finish()
{
    for (int i = 0; i < _netSize; i++)
        delete _users[i];

    delete [] _users;
    delete _eNodeB;
}
