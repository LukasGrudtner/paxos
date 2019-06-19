#include "../include/learner.h"

Learner::Learner(unsigned int id)
{
    this->_id = id;
    read_nodes_address();
}

int Learner::chosen_value()
{
    return _chosen_v;
}

void Learner::start_listening(unsigned short port_num)
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

std::string Learner::on_received_response(std::string response)
{
    if (response != "")
    {
        if (_chosen)
        {
            std::cout << "CHOSEN VALUE: " << _chosen_v << ", REQUEST IGNORED" << std::endl;
            return "";
        }

        std::cout << "LEARNER [" << _id << "] RECEIVED: " << response << std::endl;

        std::vector<std::string> info;
        boost::split(info, response, boost::is_any_of(SEPARATOR));

        std::string request = info[0];
        int v = stoi(info[2]);

        if (request == ACCEPTED)
        {
            map[v] = map[v] + 1;

            if (map[v] > NUMBER_OF_ACCEPTORS/2) {
                _chosen_v = v;
                _chosen = true;
                std::cout << "CHOSEN V: " << v << "(ACCEPTED BY " << map[v] << " ACCEPTORS)" << std::endl;
            }
        }
    }
}

void Learner::read_nodes_address()
{
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

int main()
{
    unsigned short id;
    unsigned short port;

    std::string answer;
    std::cout << "Digite o ID do LEARNER: ";
    std::cin >> answer;
    id = stoi(answer);

    std::cout << "Digite a PORTA do LEARNER: ";
    std::cin >> answer;
    port = stoi(answer);

    Learner learner = Learner(id);
    learner.start_listening(port);
}