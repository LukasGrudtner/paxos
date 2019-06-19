#ifndef __PROPOSER_H__
#define __PROPOSER_H__

#include <string>
#include <fstream>
#include "settings.h"
#include "client_socket.h"
#include "paxos_component.h"

class Proposer : public PaxosComponent
{
public:
    Proposer(unsigned int id);
    /**
     * O número da proposta de cada proceso proponente (proposer) deve ser um
     * número natural único, positivo e monotônico, com relação aos números
     * de proposta de outros proponentes.
     */
    unsigned int proposal_number();

    /**
     * Processos que executam os papéis de 'proposers':
     * O processo envia mensagem a todos os processos 'acceptors' pedindo que aceitem
     * a dupla (n, v), onde n representa o número da proposta e v um dado valor
     * (enviado por um processo cliente).
     */
    void prepare_request(unsigned int n, unsigned int v);

    /**
     * Depois de receber a confirmação 'prepare_response' da maioria dos acceptors,
     * o processo retorna esta mensagem à eles com o N definido anteriormente por ele
     * e o maior V que ele recebeu nas mensagens prepare_response.
     */
    void accept_request();

    std::string on_received_response(std::string response);

private:
    unsigned int _num_accepted = 0;
    unsigned int _n = 0;
    unsigned int _v = 0;
    std::vector<Component> _acceptors;
};

#endif