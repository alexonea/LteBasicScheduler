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

#ifndef NODES_CONFIGURATOR_CONFIGURATORINTERFACE_H_
#define NODES_CONFIGURATOR_CONFIGURATORINTERFACE_H_

#include <omnetpp.h>

using namespace omnetpp;

class ConfiguratorInterface
{
public:
    ConfiguratorInterface() {};
    virtual ~ConfiguratorInterface() {};

    virtual cGate* commandGetENBControlEndpoint(int userId) = 0;
    virtual cGate* commandGetENBUplinkEndpoint(int userId) = 0;
    virtual cGate* commandGetUserControlEndpoint(int userId) = 0;
    virtual cGate* commandGetUserDownlinkEndpoint(int userId) = 0;

    static ConfiguratorInterface* commandGetConfiguratorInstance(cModule *current);
};

#endif /* NODES_CONFIGURATOR_CONFIGURATORINTERFACE_H_ */
