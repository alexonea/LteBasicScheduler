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

void ProportionalFairSchedulingScheme::_resetRBAllocationStatus()
{
    for (int i = 0; i < _numRBs; i++)
        _RBAlreadyAllocated[i] = false;
}

ProportionalFairSchedulingScheme::ProportionalFairSchedulingScheme(int numRBs) : SchedulingScheme(numRBs)
{
    this->_scoreBoard = new double*[numRBs]();
    this->_RBAlreadyAllocated = new bool[numRBs];
    this->_numUsers = -1;
    this->_lastAllocationForUser = nullptr;
}

ProportionalFairSchedulingScheme::~ProportionalFairSchedulingScheme()
{
    delete [] _RBAlreadyAllocated;

    for (int i = 0; i < _numRBs; i++)
    {
        if (_scoreBoard[i] != nullptr)
            delete [] _scoreBoard[i];
    }

    delete [] _scoreBoard;

    if (_lastAllocationForUser != nullptr)
        delete [] _lastAllocationForUser;
}

bool ProportionalFairSchedulingScheme::_isAdjacent(int RB, int userId)
{
    bool notFound = true;
    for (int i = 0; i < _numRBs; i++)
    {
        if (_RBAlreadyAllocated[i] && _schedTable[i] == userId)
        {
            notFound = false;
            if (abs(RB - i) == 1)
                return true;
        }
    }

    return notFound;
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

    /* treat special case in which just one user needs serving */
    if (queue.size() == _numRBs)
    {
        int userId = queue.top().userId;

        for (int i = 0; i < _numRBs; i++)
        {
            decision->allocateToUser(userId, i, 0);
            decision->allocateToUser(userId, i, 1);
        }

        return decision;
    }

    /*
     * Implementation of riding peak proportional fair uplink scheduling algorithm
     * http://staff.cs.upt.ro/~todinca/mcs/Articles/Lte/ucla_3gpplteuplink.pdf
     */

    for (int timeslot = 0; timeslot < 2; timeslot++)
    {
        _resetRBAllocationStatus();

        int leftRBs = _numRBs;
        int k = 0;

        while (leftRBs > 0)
        {
            UserRBScore current = queue.get(k);

            if (_isAdjacent(current.RB, current.userId))
            {
                _schedTable[current.RB] = current.userId;
                _RBAlreadyAllocated[current.RB] = true;

                queue.removeAllForRB(current.RB);
                /* queue.remove(current); */

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

        for (int i = 0; i < _numRBs; i++)
        {
            decision->allocateToUser(_schedTable[i], i, timeslot);
        }
    }

    return decision;
}
