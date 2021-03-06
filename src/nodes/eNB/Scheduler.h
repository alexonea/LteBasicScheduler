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

#ifndef __LTEBASICSCHEDULER_SCHEDULER_H_
#define __LTEBASICSCHEDULER_SCHEDULER_H_

#include <omnetpp.h>

#include "../common/UserInfoInterface.h"
#include "../configurator/ConfiguratorInterface.h"
#include "SchedulingScheme.h"
#include "ChannelQualityReportingInterface.h"

using namespace omnetpp;

class Scheduler : public cSimpleModule, public ChannelQualityReportingInterface
{
private:
    double _schedCycle;
    int _numConnections;
    int _numRBs;
    SchedulingScheme *_schedulingScheme;
    UserInfo *_userInfo;
    UserInfoInterface **_userManager;
    simsignal_t *_signalUserAllocation;
    double **_channelQuality;
    ConfiguratorInterface *_config;

    void _readUserInfo();
    void _printChannelQuality();
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
public:
    virtual void commandUpdateChannelQuality(int userId, int RB, double value) override;
};

#endif
