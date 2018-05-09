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

#include "Transciever.h"

#define RE_PER_RB 12
#define QPSK 0
#define QAM16 1
#define QAM64 2

#define PACKET_SIZE 1024

static const int BITS_PER_SYMBOL[3] = { 2, 4, 6 };

Define_Module(Transciever);

void Transciever::initialize()
{
    this->_bandwidth = par("bandwidth");
    this->_symbolsPerRE = par("extCP") ? 6 : 7;
}

void Transciever::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        delete msg;
    }
    else if (msg->arrivedOn("dataTX"))
    {
        ResourceBlock **RBList;
        DataPacket *packet = static_cast <DataPacket *> (msg);
        int totalRBs;

        RBList = this->commandEncode(packet, totalRBs);
        EV << "Total RBs " << totalRBs << endl;

        for (int i = 0; i < totalRBs; i++)
        {
            send(RBList[i], "RBTX");
        }

        delete packet;
    }
    else if (msg->arrivedOn("RBRX"))
    {
        // message from downlink channel
        // temporarily delete the message
        delete msg;
    }
}

ResourceBlock** Transciever::commandEncode(DataPacket *data, int &totalRBs)
{
    ResourceBlock **RBList;
    const int senderId = data->getSenderId();
    const int size = data->getSize();

    /* for now we use QPSK for both UL and DL */
    const int bitsPerRB = RE_PER_RB * _symbolsPerRE * BITS_PER_SYMBOL[QPSK];

    /* 8 bits per byte of data */
    const int totalBits = size * 8;

    /* round up to an integer number of RBs */
    totalRBs = 1 + ((totalBits - 1) / bitsPerRB);

    RBList = new ResourceBlock*[totalRBs];
    for (int i = 0; i < totalRBs; i++)
    {
        RBList[i] = new ResourceBlock("RB");
        RBList[i]->setSenderId(senderId);
    }

    return RBList;
}
