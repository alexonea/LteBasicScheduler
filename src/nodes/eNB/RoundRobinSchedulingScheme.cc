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

#include "RoundRobinSchedulingScheme.h"

#include <omnetpp.h>

using namespace omnetpp;

RoundRobinSchedulingScheme::RoundRobinSchedulingScheme(int numUsers)
{
    this->_numUsers = numUsers;
    this->_numRBs = 30;
    this->_fixedAllocationSize = 7;

    this->_schedTable = new int[_numRBs];
}

SchedulingDecision* RoundRobinSchedulingScheme::schedule()
{
    static int currentUserId = 0;
    static int currentAllocationPos = 0;
    static int currentSchedulingCycle = 0;

    SchedulingDecision *decision = new SchedulingDecision(_numUsers);

    for (int timeslot = 0; timeslot < 2; timeslot++)
    {
           for (int currentRB = 0; currentRB < _numRBs; currentRB++)
           {
               if (currentAllocationPos >= _fixedAllocationSize)
               {
                   currentAllocationPos = 0;
                   currentUserId = (currentUserId + 1) % _numUsers;
               }

               _schedTable[currentRB] = currentUserId;
               currentAllocationPos++;
           }

           EV << "Timeslot " << timeslot << ": ";

           /* update the current schedule */
           for (int currentRB = 0; currentRB < _numRBs; currentRB++)
           {
               int userId = _schedTable[currentRB];
               decision->allocateToUser(userId, currentRB, timeslot);

               EV << userId << " ";
           }

           EV << endl;
    }

    return decision;
}

RoundRobinSchedulingScheme::~RoundRobinSchedulingScheme()
{
    delete [] _schedTable;
}
