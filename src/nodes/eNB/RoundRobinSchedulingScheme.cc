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

RoundRobinSchedulingScheme::RoundRobinSchedulingScheme()
{
    this->_numRBs = 30;
    this->_fixedAllocationSize = 4;

    this->_schedTable = new int[_numRBs];
}

int RoundRobinSchedulingScheme::_findNextUser(int currentUserId, int numUsers, UserInfo *userInfo)
{
    int count = 0;

    EV << "Finding next user after " << currentUserId << endl;
    currentUserId = (currentUserId + 1) % numUsers;

    if (userInfo != nullptr)
    {

        while (userInfo[currentUserId].queueLength == 0 && count < numUsers)
        {
            EV << "Trying " << currentUserId << "... NOK" << endl;
            currentUserId = (currentUserId + 1) % numUsers;
            count++;
        }

        /* if no users have to send, return null */
        if (count == numUsers)
            return -1;

        EV << "User " << currentUserId << " selected!" << endl;
    }

    return currentUserId;
}

SchedulingDecision* RoundRobinSchedulingScheme::schedule(int numUsers, UserInfo *userInfo)
{
    static int currentUserId = 0;
    static int currentAllocationPos = 0;

    SchedulingDecision *decision = new SchedulingDecision(numUsers);

    /* find the first user which has a non empty queue */
    currentUserId = _findNextUser(-1, numUsers, userInfo);

    if (currentUserId == -1)
        return nullptr;

    for (int timeslot = 0; timeslot < 2; timeslot++)
    {
           for (int currentRB = 0; currentRB < _numRBs; currentRB++)
           {
               if (currentAllocationPos >= _fixedAllocationSize)
               {
                   currentAllocationPos = 0;
                   currentUserId = _findNextUser(currentUserId, numUsers, userInfo);
               }

               _schedTable[currentRB] = currentUserId;
               currentAllocationPos++;
           }

           /* update the current schedule */
           for (int currentRB = 0; currentRB < _numRBs; currentRB++)
           {
               int userId = _schedTable[currentRB];
               decision->allocateToUser(userId, currentRB, timeslot);
           }
    }

    return decision;
}

RoundRobinSchedulingScheme::~RoundRobinSchedulingScheme()
{
    delete [] _schedTable;
}
