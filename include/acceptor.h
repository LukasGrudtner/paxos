#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include <thread>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/algorithm/string.hpp>

#include "paxos_component.h"
#include "server_socket.h"
#include "client_socket.h"
#include "settings.h"

class Acceptor : public PaxosComponent
{
public:
    Acceptor(unsigned int id);

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
    void accepted();

    /**
     * Inicia um socket server para receber mensagens.
     */
    void start_listening(unsigned short port);

    std::string on_received_response(std::string response);


private:
    // static std::string on_response_received(std::string response);

private:
    unsigned int _chosen_n   = 0;
    int          _chosen_v   = 0;
    std::vector<Component> _learners;
    bool _accepted = false;
};

#endif