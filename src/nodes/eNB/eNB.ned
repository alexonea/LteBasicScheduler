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

package ltebasicscheduler.nodes.eNB;

module eNB
{
	parameters:
		int maxConnections = default(100);
		int schedulingScheme = default(0);	// use 0 for round-robin, 1 for proportional-fair
		scheduler.schedulingScheme = schedulingScheme;
	gates:
		inout ctrl[maxConnections] @loose;
		input ulink[maxConnections] @loose;
	submodules:
		sink : Sink { size = maxConnections; };
		scheduler : Scheduler { size = maxConnections; };
	connections:
		for i = 0..(maxConnections - 1)
		{
			ulink++ --> sink.drain++;
			ctrl++ <--> scheduler.ctrl++;
		}
}
