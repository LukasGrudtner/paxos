#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <string>
#include <stdio.h>
#include <time.h>

#define MAX_TIME 5000   /* Tempo máximo em milisegundos de espera no tempo de processamento. */
#define MAX_N 10000     /* Valor máximo na escolha do n */

#define DEFAULT_PORT 1234   /* Porta padrão */
#define DEFAULT_THREAD_POOL_SIZE 2  /* Tamanho padrão do pool de threads */

#define NUMBER_OF_ACCEPTORS 2   /* Número de acceptors no sistema */
#define NUMBER_OF_PROPOSERS 2   /* Número de proposers no sistema */
#define NUMBER_OF_LEARNERS 2    /* Número de learners no sistema */

#define SEPARATOR ";"       /* Separador de informações nas mensagens enviadas */
#define END_OF_MESSAGE "\n" /* Símbolo de fim de mensagem */

/* Messages */
#define PREPARE_REQUEST     "prepare_request"   /* Mensagem de prepare request */
#define PREPARE_RESPONSE    "prepare_response"  /* Mensagem de prepare response */
#define ACCEPT_REQUEST      "accept_request"    /* Mensagem de accept request */
#define ACCEPTED            "accepted"          /* Mensagem de accepted */

/* Paxos components */
#define PROPOSER "proposer"     /* Descrição dos componentes */
#define ACCEPTOR "acceptor"
#define LEARNER "learner"

/* Estrutura com a informação de cada um dos nós */
struct Component {
    std::string component;
    std::string ip_address;
    int port_number;
};

#endif