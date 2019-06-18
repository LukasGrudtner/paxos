#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include <thread>
#include <boost/asio.hpp>

#include "server_socket.h"
#include "settings.h"

class Acceptor
{
public:
    Acceptor();

    /**
     * Se o acceptor não tiver recebido nenhuma proposta, ele envia uma mensagem
     * prometendo não aceitar outras propostas com números n abaixo daquele informado
     * na mensagem.
     * Senão, se o receptor já tiver recebido uma solicitação com um número de proposta
     * maior, a solicitação atual é ignorada. Se o receptor já tiver recebido uma
     * solicitação com um número de proposta menor, ele retorna ao proposer o número
     * da solicitação e o valor associado à ela.
     */
    int prepare_response();

    /**
     * Recebe a mensagem 'accept_request' e envia 'accepted' para os processos learners,
     * os quais executam a solicitação e enviam uma resposta ao cliente.
     */
    int accepted();

    /**
     * Inicia um socket server para receber mensagens.
     */
    void start_listening();
private:
};

#endif