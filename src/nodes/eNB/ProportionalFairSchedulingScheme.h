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

#ifndef NODES_ENB_PROPORTIONALFAIRSCHEDULINGSCHEME_H_
#define NODES_ENB_PROPORTIONALFAIRSCHEDULINGSCHEME_H_

#include "SchedulingScheme.h"

#include <functional>
#include <queue>
#include <vector>
#include <algorithm>

typedef struct _UserRBScore
{
    int userId;
    int RB;
    double score;

    _UserRBScore() {};
    _UserRBScore(int userId, int RB, double score)
    {
        this->userId = userId;
        this->RB = RB;
        this->score = score;
    }

    bool operator==(const _UserRBScore &other) const
    {
        return (this->RB == other.RB && this->score == other.score && this->userId == other.userId);
    }
} UserRBScore;

template<
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type>
>
class score_priority_queue : public std::priority_queue<T, Container, Compare>
{
public:
    explicit score_priority_queue(const Compare& x) : std::priority_queue<T, Container, Compare>(x) {};

    bool remove(const T& value)
    {
        auto it = std::find(this->c.begin(), this->c.end(), value);
        if (it != this->c.end())
        {
            this->c.erase(it);
            std::make_heap(this->c.begin(), this->c.end(), this->comp);
            return true;
        }
        else
        {
            return false;
        }
    }

    const T& get(int index)
    {
        return this->c[index];
    }
};

class ProportionalFairSchedulingScheme: public SchedulingScheme
{
public:
    ProportionalFairSchedulingScheme(int numRBs);
    virtual ~ProportionalFairSchedulingScheme();

    virtual SchedulingDecision* schedule(int numUsers, UserInfo *userInfo) override;
private:
    int _numUsers;
    double **_scoreBoard;
    int *_lastAllocationForUser;

    static bool _isAdjacent(SchUserAllocation allocation, int RB, int timeslot);
};

#endif /* NODES_ENB_PROPORTIONALFAIRSCHEDULINGSCHEME_H_ */
