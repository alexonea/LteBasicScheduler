/*
 * ConfiguratorInterface.cc
 *
 *  Created on: May 15, 2018
 *      Author: uids4871
 */

#include "ConfiguratorInterface.h"

ConfiguratorInterface* ConfiguratorInterface::commandGetConfiguratorInstance(cModule *current)
{
    while (current->getParentModule() != nullptr)
        current = current->getParentModule();

    return check_and_cast <ConfiguratorInterface *> (current->getSubmodule("configurator"));
}
