//
// Generated file, do not edit! Created by nedtool 5.3 from messages/DataPacket.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __DATAPACKET_M_H
#define __DATAPACKET_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0503
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>messages/DataPacket.msg:19</tt> by nedtool.
 * <pre>
 * //
 * // TODO generated message class
 * //
 * packet DataPacket
 * {
 *     int senderId;
 *     unsigned char payload[1024];
 * }
 * </pre>
 */
class DataPacket : public ::omnetpp::cPacket
{
  protected:
    int senderId;
    unsigned char payload[1024];

  private:
    void copy(const DataPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const DataPacket&);

  public:
    DataPacket(const char *name=nullptr, short kind=0);
    DataPacket(const DataPacket& other);
    virtual ~DataPacket();
    DataPacket& operator=(const DataPacket& other);
    virtual DataPacket *dup() const override {return new DataPacket(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getSenderId() const;
    virtual void setSenderId(int senderId);
    virtual unsigned int getPayloadArraySize() const;
    virtual unsigned char getPayload(unsigned int k) const;
    virtual void setPayload(unsigned int k, unsigned char payload);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const DataPacket& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, DataPacket& obj) {obj.parsimUnpack(b);}


#endif // ifndef __DATAPACKET_M_H

