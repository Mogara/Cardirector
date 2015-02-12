#include "cabstractpacket.h"

class CAbstractPacketPrivate
{
public:
    int command;
    QVariant data;
};

CAbstractPacket::CAbstractPacket(int command)
    : p_ptr(new CAbstractPacketPrivate)
{
    p_ptr->command = command;
}

CAbstractPacket::~CAbstractPacket()
{
    delete p_ptr;
}

int CAbstractPacket::command() const
{
    return p_ptr->command;
}

void CAbstractPacket::setData(const QVariant &data)
{
    p_ptr->data = data;
}

const QVariant &CAbstractPacket::data() const
{
    return p_ptr->data;
}
