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

#include "SchedulingDecision.h"

class RoundRobinSchedulingScheme
{
public:
    RoundRobinSchedulingScheme(int numUsers);
    virtual ~RoundRobinSchedulingScheme();

    SchedulingDecision* schedule();
private:
    int _numUsers;
    int _numRBs;
    int _fixedAllocationSize;
    int *_schedTable;
};

#endif /* NODES_ENB_ROUNDROBINSCHEDULINGSCHEME_H_ */
