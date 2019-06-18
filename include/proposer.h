#ifndef __PROPOSER_H__
#define __PROPOSER_H__

#include <string>
#include "settings.h"
#include "client_socket.h"

class Proposer
{
public:
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
    void prepare_request();

    /**
     * Depois de receber a confirmação 'prepare_response' da maioria dos acceptors,
     * o processo retorna esta mensagem à eles com o N definido anteriormente por ele
     * e o maior V que ele recebeu nas mensagens prepare_response.
     */
    int accept_request();

private:

};

#endif