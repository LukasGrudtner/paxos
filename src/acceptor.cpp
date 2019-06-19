#include "../include/acceptor.h"

Acceptor::Acceptor(unsigned int id)
{
    this->_id = id;
    read_nodes_address();    
}

void Acceptor::start_listening(unsigned short port_num)
{
    try {
        ServerSocket socket;
        socket.start(port_num, DEFAULT_THREAD_POOL_SIZE, this);

        while (1) std::this_thread::sleep_for(std::chrono::seconds(60));
        socket.stop();
    }
    catch (system::system_error& e) {
        std::cout << "Error ocurred! Error code = " << e.code() << ". Message: " << e.what();
    }
}

std::string Acceptor::on_received_response(std::string response)
{
    if (response != "") {

        if (_accepted)
        {
            std::cout << "CHOSEN VALUE: " << _chosen_v << ", REQUEST IGNORED" << std::endl;
            return "";
        }

        std::cout << "ACCEPTOR [" << _id << "] RECEIVED: " << response;

        std::vector<std::string> info;
        boost::split(info, response, boost::is_any_of(SEPARATOR));

        std::string request = info[0];

        unsigned int n = stoi(info[1]);
        unsigned int v = stoi(info[2]);
        if (request == PREPARE_REQUEST) {

            if (n > _chosen_n) {
                _chosen_n = n;
                _chosen_v = v;

                std::cout << "ACCEPTOR [" << _id << "] SENT: " << std::string(PREPARE_RESPONSE) << std::endl;
                
                /* ...processing time... */
                srand(time(NULL));
                std::this_thread::sleep_for(std::chrono::milliseconds(((rand() % MAX_TIME) + 1000)));
                /* ...processing time... */
                
                return std::string(PREPARE_RESPONSE) + END_OF_MESSAGE;
            }
        }
        else if (request == ACCEPT_REQUEST) {
            if (n >= _chosen_n) {
                _chosen_n = n;
                _chosen_v = v;

                accepted();
                return "";
            }
        }

        std::cout << "REQUEST IGNORED" << std::endl;
    }

    return "";
}

void Acceptor::accepted()
{
    _accepted = true;
    std::string request = std::string(ACCEPTED) + SEPARATOR +
                            std::to_string(_chosen_n) + SEPARATOR +
                            std::to_string(_chosen_v) + SEPARATOR +
                            END_OF_MESSAGE;

    for (unsigned int i = 0; i < _learners.size(); i++) {

        /* ...processing time... */
        srand(time(NULL));
        std::this_thread::sleep_for(std::chrono::milliseconds(((rand() % MAX_TIME) + 1000)));
        /* ...processing time... */

        try {
            ClientSocket socket;
            std::cout << "ACCEPTOR [" << _id << "] SENT " << ACCEPTED << SEPARATOR << _chosen_n << SEPARATOR << _chosen_v << " TO " << _learners[i].ip_address << ":" << _learners[i].port_number << std::endl;
            socket.send(request, 10, _learners[i].ip_address, _learners[i].port_number, this, i);
            socket.close();
        }
        catch (system::system_error &e)
        {
            std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what();
        }
    }
}

void Acceptor::read_nodes_address()
{
    std::ifstream ifs;
    ifs.open("learners.info");

    std::string ip_address;
    std::string port_num;
    
    while (!ifs.eof())
    {
        ifs >> ip_address;
        ifs >> port_num;

        _learners.push_back({"", ip_address, stoi(port_num)});
    }
}

int main()
{
    unsigned short id;
    unsigned short port;

    std::string answer;
    std::cout << "Digite o ID do ACCEPTOR: ";
    std::cin >> answer;
    id = stoi(answer);

    std::cout << "Digite a PORTA do ACCEPTOR: ";
    std::cin >> answer;
    port = stoi(answer);

    Acceptor acceptor = Acceptor(id);
    acceptor.start_listening(port);
}