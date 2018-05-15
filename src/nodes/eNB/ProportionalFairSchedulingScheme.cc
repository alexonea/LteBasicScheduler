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

ProportionalFairSchedulingScheme::ProportionalFairSchedulingScheme(int numRBs, int numUsers) : SchedulingScheme(numRBs, numUsers)
{
    this->_scoreBoard = new double*[numRBs]();
    this->_RBAlreadyAllocated = new bool[numRBs];
    this->_lastAllocationForUser = new int[numUsers]();

    for (int i = 0; i < numUsers; i++)
        this->_lastAllocationForUser[i] = 5;

    for (int i = 0; i < _numRBs; i++)
        this->_scoreBoard[i] = new double[numUsers]();
}

ProportionalFairSchedulingScheme::~ProportionalFairSchedulingScheme()
{
    delete [] _RBAlreadyAllocated;

    for (int i = 0; i < _numRBs; i++)
        delete [] _scoreBoard[i];

    delete [] _scoreBoard;
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

void ProportionalFairSchedulingScheme::_updateScores(UserInfo *userInfo)
{
    _scores.clear();

    for (int i = 0; i < _numRBs; i++)
    {
        for (int j = 0; j < _numUsers; j++)
        {
            _scoreBoard[i][j] = userInfo[j].channelQuality[i] * (double) _lastAllocationForUser[j];

            if (userInfo[j].queueLength != 0)
                _scores.push_back(UserRBScore(j, i, _scoreBoard[i][j]));
        }
    }

    std::sort(_scores.begin(), _scores.end(), UserRBScoreComparator());
}

int ProportionalFairSchedulingScheme::_getNthScorePos(int n)
{
    while (n < _scores.size())
    {
        UserRBScore c = _scores[n];

        if (!_RBAlreadyAllocated[c.RB])
            return n;

        n++;
    }

    return -1;
}

void ProportionalFairSchedulingScheme::_removeFromScoreList(int n)
{
   if (n >= 0 && n < _scores.size())
   {
       _scores.erase(_scores.begin() + n);
   }
}

SchedulingDecision* ProportionalFairSchedulingScheme::schedule(int numUsers, UserInfo *userInfo)
{
    _updateScores(userInfo);

    /* if no user needs to transmit, return null */
    if (_scores.size() == 0)
        return nullptr;

    SchedulingDecision *decision = new SchedulingDecision(numUsers);

    /*
     * Implementation of riding peak proportional fair uplink scheduling algorithm
     * http://staff.cs.upt.ro/~todinca/mcs/Articles/Lte/ucla_3gpplteuplink.pdf
     */

    for (int timeslot = 0; timeslot < 2; timeslot++)
    {
        _resetRBAllocationStatus();

        if (_scores.size() == 0)
            _updateScores(userInfo);

        int leftRBs = _numRBs;
        int k = 0;

        while (leftRBs > 0)
        {
            int pos = _getNthScorePos(k);
            if (pos < 0)
            {
                EV << "This should not happen!" << endl;
                break;
            }

            UserRBScore current = _scores[pos];

            if (_isAdjacent(current.RB, current.userId))
            {
                _schedTable[current.RB] = current.userId;
                _RBAlreadyAllocated[current.RB] = true;

                _removeFromScoreList(pos);

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
