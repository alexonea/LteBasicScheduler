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

#include "ProportionalFairSchedulingScheme.h"

#include <cmath>
#include <omnetpp.h>

using namespace omnetpp;

ProportionalFairSchedulingScheme::ProportionalFairSchedulingScheme(int numRBs) : SchedulingScheme(numRBs)
{
    this->_scoreBoard = new double*[numRBs]();
    this->_numUsers = -1;
    this->_lastAllocationForUser = nullptr;
}

ProportionalFairSchedulingScheme::~ProportionalFairSchedulingScheme()
{
    for (int i = 0; i < _numRBs; i++)
    {
        if (_scoreBoard[i] != nullptr)
            delete [] _scoreBoard[i];
    }

    delete [] _scoreBoard;

    if (_lastAllocationForUser != nullptr)
        delete [] _lastAllocationForUser;
}

bool ProportionalFairSchedulingScheme::_isAdjacent(SchUserAllocation allocation, int RB, int timeslot)
{
    for (int i = 0; i < allocation.count; i++)
    {
        if (allocation.RBs[i].timeslot == timeslot && abs(allocation.RBs[i].RB - RB) == 1)
            return true;
    }

    return false;
}

SchedulingDecision* ProportionalFairSchedulingScheme::schedule(int numUsers, UserInfo *userInfo)
{
    if (_numUsers == -1)
        _numUsers = numUsers;

    if (_numUsers < numUsers)
    {
        delete [] _lastAllocationForUser;
        _lastAllocationForUser = nullptr;
        _numUsers = numUsers;
    }

    /* if this is the first schedule cycle, allocate space for internal data */
    if (_lastAllocationForUser == nullptr)
    {
        _lastAllocationForUser = new int[numUsers]();

        for (int i = 0; i < numUsers; i++)
            _lastAllocationForUser[i] = 5;
    }

    auto cmp = [](UserRBScore a, UserRBScore b) { if (a.score == b.score) return (a.RB < b.RB); else return (a.score < b.score); };
    score_priority_queue<UserRBScore, std::vector<UserRBScore>, decltype(cmp)> queue(cmp);

    for (int i = 0; i < _numRBs; i++)
    {
        if (_scoreBoard[i] == nullptr)
            _scoreBoard[i] = new double[numUsers]();

        for (int j = 0; j < numUsers; j++)
        {
            if (userInfo[j].queueLength != 0)
            {
                _scoreBoard[i][j] = userInfo[j].channelQuality[i] * (double) _lastAllocationForUser[j];
                queue.push(UserRBScore(j, i, _scoreBoard[i][j]));
            }
        }
    }

    /* if no user needs to transmit, return null */
    if (queue.size() == 0)
        return nullptr;

    SchedulingDecision *decision = new SchedulingDecision(numUsers);

    /*
     * Implementation of riding peak proportional fair uplink scheduling algorithm
     * http://staff.cs.upt.ro/~todinca/mcs/Articles/Lte/ucla_3gpplteuplink.pdf
     */

    for (int timeslot = 0; timeslot < 2; timeslot++)
    {
        bool *alreadyAllocated = new bool[_numRBs]();
        for (int i = 0; i < _numRBs; i++)
            alreadyAllocated[i] = false;

        int leftRBs = _numRBs;
        int k = 0;

        while (leftRBs > 0)
        {
            UserRBScore current;
            int q = k;

            do
            {
                if (q >= queue.size())
                    break;

                current = queue.get(q);
                if (!alreadyAllocated[current.RB])
                    break;

                q++;
            } while (alreadyAllocated[current.RB]);

            if (q == queue.size())
            {
                break;
            }

            SchUserAllocation currentAllocation = decision->getAllocationForUser(current.userId);

            if (_isAdjacent(currentAllocation, current.RB, timeslot) || currentAllocation.count == 0)
            {
                decision->allocateToUser(current.userId, current.RB, timeslot);
                queue.remove(current);

                k = 0;
                leftRBs--;

                _lastAllocationForUser[current.userId] --;

                if (_lastAllocationForUser[current.userId] == 0)
                    _lastAllocationForUser[current.userId] = 5;
            }
            else
            {
                k++;
            }
        }

        delete [] alreadyAllocated;
    }

    return decision;
}
