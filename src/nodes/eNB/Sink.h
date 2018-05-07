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

using namespace omnetpp;

typedef struct _UserStats
{
    unsigned int totalRBs;
    unsigned int RBsSinceLastTimeUnit;
    unsigned int avrgDatarate;
    unsigned int maxDatarate;
    unsigned int minDatarate;
    double instDatarate;
    unsigned int avrgDelay;
    unsigned int maxDelay;
    unsigned int minDelay;
    unsigned int lastRBTimestamp;
} UserStats;

class Sink : public cSimpleModule
{
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
private:
    unsigned int _numUsers;
    double _statsUpdateCycle;
    UserStats *_userStats;
    simsignal_t *_signalUserRBs;
};

#endif
