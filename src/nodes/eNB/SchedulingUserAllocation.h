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

#ifndef NODES_ENB_SCHEDULINGUSERALLOCATION_H_
#define NODES_ENB_SCHEDULINGUSERALLOCATION_H_

#include <vector>

typedef struct _RBAllocation
{
    int RB;
    int timeslot;
} RBAllocation;

typedef struct _SchUserAllocation
{
    int count;
    std::vector<RBAllocation> RBs;

    _SchUserAllocation()
    {
        count = 0;
        RBs = std::vector<RBAllocation>();
    }
} SchUserAllocation;

#endif /* NODES_ENB_SCHEDULINGUSERALLOCATION_H_ */
