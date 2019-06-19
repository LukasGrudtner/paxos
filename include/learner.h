#ifndef __LEARNER_H__
#define __LEARNER_H__

#include <map>
#include <thread>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

#include "settings.h"
#include "server_socket.h"
#include "paxos_component.h"

class Learner : public PaxosComponent
{
public:
    Learner(unsigned int id);

    void start_listening(unsigned short port_num);
    int chosen_value();

    std::string on_received_response(std::string response);

private:
    int _chosen_v = 0;
    std::vector<Component> _acceptors;
    bool _chosen = false;
    std::map<unsigned int, int> map;
};

#endif