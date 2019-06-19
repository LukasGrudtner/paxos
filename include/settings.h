#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <string>
#include <stdio.h>
#include <time.h>

#define MAX_TIME 5000
#define MAX_N 10000

#define DEFAULT_PORT 1234
#define DEFAULT_THREAD_POOL_SIZE 2

#define NUMBER_OF_ACCEPTORS 4
#define NUMBER_OF_PROPOSERS 2
#define NUMBER_OF_LEARNERS 3

#define SEPARATOR ";"
#define END_OF_MESSAGE "\n"

/* Messages */
#define PREPARE_REQUEST     "prepare_request"
#define PREPARE_RESPONSE    "prepare_response"
#define ACCEPT_REQUEST      "accept_request"
#define ACCEPTED            "accepted"

/* Paxos components */
#define PROPOSER "proposer"
#define ACCEPTOR "acceptor"
#define LEARNER "learner"

/* Component info struct */
struct Component {
    std::string component;
    std::string ip_address;
    int port_number;
};

#endif