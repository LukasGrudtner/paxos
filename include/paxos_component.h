#ifndef __PAXOS_COMPONENT_H__
#define __PAXOS_COMPONENT_H__

#include <string>

class PaxosComponent
{
public:
    PaxosComponent();
    
    /* Método virtual implementado nas classes filhas. */
    virtual std::string on_received_response(std::string response) = 0;

protected:
    /* Identificador do componente Paxos (acceptor, proposer ou learner). */
    unsigned int _id = 0;
};

#endif