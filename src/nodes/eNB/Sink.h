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

#ifndef __LTEBASICSCHEDULER_SINK_H_
#define __LTEBASICSCHEDULER_SINK_H_

#include <omnetpp.h>

#include "ChannelQualityReportingInterface.h"

using namespace omnetpp;

typedef struct _UserStats
{
    unsigned long int totalRBs;
    unsigned long int bitsSinceLastTimeUnit;
    double instDatarate;
    unsigned int lastRBTimestamp;
} UserStats;

class Sink : public cSimpleModule
{
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
private:
    unsigned int _numUsers;
    double _statsUpdateCycle;
    UserStats *_userStats;
    simsignal_t *_signalUserRBs;
    ChannelQualityReportingInterface *_channelQualityManager;
};

#endif
