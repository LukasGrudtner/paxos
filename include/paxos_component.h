#ifndef __PAXOS_COMPONENT_H__
#define __PAXOS_COMPONENT_H__

#include <string>

class PaxosComponent
{
public:
    PaxosComponent();
    virtual std::string on_received_response(std::string response) = 0;

protected:
    unsigned int _id = 0;
};

#endif