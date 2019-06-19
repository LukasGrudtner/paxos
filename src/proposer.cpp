#include "../include/proposer.h"

Proposer::Proposer(unsigned int id)
{
    this->_id = id;

    std::ifstream ifs;
    ifs.open("acceptors.info");

    std::string ip_address;
    std::string port_num;
    
    while (!ifs.eof())
    {
        ifs >> ip_address;
        ifs >> port_num;

        _acceptors.push_back({"", ip_address, stoi(port_num)});
    } 
}

void Proposer::prepare_request(unsigned int n, unsigned int v)
{
    _n = n;
    _v = v;

    std::string request = std::string(PREPARE_REQUEST) + SEPARATOR +
                            std::to_string(n) + SEPARATOR +
                            std::to_string(v) + SEPARATOR +
                            END_OF_MESSAGE;

    for (int i = 0; i < _acceptors.size(); i++) {
        
        /* ...processing time... */
        srand(time(NULL));
        std::this_thread::sleep_for(std::chrono::milliseconds(((rand() % MAX_TIME) + 1000)));
        /* ...processing time... */

        try {
            ClientSocket socket;
            std::cout << "PROPOSER [" << _id << "] SENT " << PREPARE_REQUEST << SEPARATOR << n << SEPARATOR << v << " TO " << _acceptors[i].ip_address << ":" << _acceptors[i].port_number << std::endl;
            socket.send(request, 10, _acceptors[i].ip_address, _acceptors[i].port_number, this, i);
            socket.close();
        }
        catch (system::system_error &e)
        {
            std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what();
        }
    }
}

unsigned int Proposer::proposal_number()
{
    srand(time(NULL));
    return rand();
}

std::string Proposer::on_received_response(std::string response)
{
    if (response != "")
        std::cout << "PROPOSER [" << _id << "] RECEIVED: " << response << std::endl;

    if (response == PREPARE_RESPONSE) {
        _num_accepted++;

        if (_num_accepted > (NUMBER_OF_ACCEPTORS/2))
            accept_request();
    }

    /* ...processing time... */
        srand(time(NULL));
        std::this_thread::sleep_for(std::chrono::milliseconds(((rand() % MAX_TIME) + 1000)));
        /* ...processing time... */

    return "";
}

void Proposer::accept_request()
{
    std::string request = std::string(ACCEPT_REQUEST) + SEPARATOR +
                            std::to_string(_n) + SEPARATOR +
                            std::to_string(_v) + SEPARATOR +
                            END_OF_MESSAGE;

    for (int i = 0; i < _acceptors.size(); i++) {
        /* ...processing time... */
        srand(time(NULL));
        std::this_thread::sleep_for(std::chrono::milliseconds(((rand() % MAX_TIME) + 1000)));
        /* ...processing time... */

        try {
            ClientSocket socket;
            std::cout << "PROPOSER [" << _id << "] SENT " << ACCEPT_REQUEST << SEPARATOR << _n << SEPARATOR << _v << " TO " << _acceptors[i].ip_address << ":" << _acceptors[i].port_number << std::endl;
            socket.send(request, 10, _acceptors[i].ip_address, _acceptors[i].port_number, this, i);
            socket.close();
        }
        catch (system::system_error &e)
        {
            std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what();
        }
    }
}

int main()
{
    unsigned short id;
    unsigned short value;

    std::string answer;
    std::cout << "Digite o ID do PROPOSER: ";
    std::cin >> answer;
    id = stoi(answer);

    std::cout << "Digite o VALOR do PROPOSER: ";
    std::cin >> answer;
    value = stoi(answer);

    Proposer proposer = Proposer(id);
    proposer.prepare_request(proposer.proposal_number() % MAX_N, value);
}