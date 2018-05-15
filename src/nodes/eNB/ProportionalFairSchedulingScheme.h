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

struct UserRBScoreComparator
{
    inline bool operator() (const UserRBScore& a, const UserRBScore& b)
    {
        return (a.score > b.score);
    }
};

template<
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type>
>
class score_priority_queue : public std::priority_queue<UserRBScore, Container, Compare>
{
public:
    explicit score_priority_queue(const Compare& x) : std::priority_queue<UserRBScore, Container, Compare>(x) {};

    bool removeAllForRB(int RB)
    {
        std::vector<std::vector<UserRBScore>::iterator> toErase;
        for (auto it = this->c.begin(); it != this->c.end(); ++it)
        {
            if (it->RB == RB)
                toErase.push_back(it);
        }

        for (auto it : toErase)
            this->c.erase(it);

        std::make_heap(this->c.begin(), this->c.end(), this->comp);
        return true;
    }

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

    const UserRBScore& get(int index)
    {
        return this->c[index];
    }
};

class ProportionalFairSchedulingScheme: public SchedulingScheme
{
public:
    ProportionalFairSchedulingScheme(int numRBs, int numUsers);
    virtual ~ProportionalFairSchedulingScheme();

    virtual SchedulingDecision* schedule(int numUsers, UserInfo *userInfo) override;
private:
    double **_scoreBoard;
    int *_lastAllocationForUser;
    bool *_RBAlreadyAllocated;
    std::vector<UserRBScore> _scores;

    int _getNthScorePos(int n);
    void _removeFromScoreList(int n);
    void _updateScores(UserInfo *userInfo);
    void _resetRBAllocationStatus();
    bool _isAdjacent(int RB, int userId);
};

#endif /* NODES_ENB_PROPORTIONALFAIRSCHEDULINGSCHEME_H_ */
