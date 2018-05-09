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

#include "SchedulingDecision.h"

SchedulingDecision::SchedulingDecision(int numUsers)
{
    this->_numUsers = numUsers;
    this->_allocation = new SchUserAllocation[_numUsers]();
}

SchedulingDecision::~SchedulingDecision()
{
    delete [] _allocation;
}

void SchedulingDecision::allocateToUser(int userId, int RB, int timeslot)
{
    RBAllocation rballoc;
    rballoc.RB = RB;
    rballoc.timeslot = timeslot;

    _allocation[userId].count++;
    _allocation[userId].RBs.push_back(rballoc);
}

SchUserAllocation SchedulingDecision::getAllocationForUser(int userId)
{
    return _allocation[userId];
}
