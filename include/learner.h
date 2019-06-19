#ifndef __LEARNER_H__
#define __LEARNER_H__

#include <map>
#include <thread>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

#include "settings.h"
#include "paxos_component.h"
#include "socket/server/server_socket.h"

class Learner : public PaxosComponent
{
public:
    Learner(unsigned int id);

    /**
     * Inicia um socket server para receber mensagens.
     */
    void start_listening(unsigned short port_num);

    /**
     * Retorna o valor escolhido pelo learner.
     */
    int chosen_value();

    /**
     * Trata as mensagens recebidas.
     */
    std::string on_received_response(std::string response);

private:

    /**
     * Lê o endereço de todos os nodos aos quais o acceptor irá se comunicar 
     * através de um arquivo de texto.
     */
    void read_nodes_address();

private:
    /* Valor v escolhido pelo learner. */
    int _chosen_v = 0;

    /* Lista de informações dos acceptors. */
    std::vector<Component> _acceptors;

    /* Flag indicando se o learner já escolheu algum valor. */
    bool _chosen = false;

    /* Mapa com o número de acceptors que aceitaram cada um dos valores v recebidos. */
    std::map<unsigned int, int> map;
};

#endif