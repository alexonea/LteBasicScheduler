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

package ltebasicscheduler.networks;

import ltebasicscheduler.nodes.UE.*;
import ltebasicscheduler.nodes.eNB.*;

network BasicNetwork
{
    parameters:
        int size = default(10);
    
    submodules:
        n[size] : UE;
        eNodeB : eNB;
    
    connections:
        for i = 0..(size - 1)
        {
			n[i].ctrl <--> { delay = uniform(10ms, 100ms); } <--> eNodeB.ctrl++;
			n[i].data --> { delay = uniform(10ms, 100ms); } --> eNodeB.ulink++;
        }
}
