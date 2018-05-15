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

#ifndef NODES_ENB_ROUNDROBINSCHEDULINGSCHEME_H_
#define NODES_ENB_ROUNDROBINSCHEDULINGSCHEME_H_

#include "../common/UserInfoInterface.h"
#include "SchedulingDecision.h"
#include "SchedulingScheme.h"


class RoundRobinSchedulingScheme : public SchedulingScheme
{
public:
    RoundRobinSchedulingScheme(int numRBs, int numUsers, int fixedAllocationSize);
    virtual ~RoundRobinSchedulingScheme();

    virtual SchedulingDecision* schedule(int numUsers, UserInfo *userInfo) override;
private:
    static int _findNextUser(int currentUserId, int numUsers, UserInfo *userInfo);

    int _fixedAllocationSize;
};

#endif /* NODES_ENB_ROUNDROBINSCHEDULINGSCHEME_H_ */
