#include "../include/acceptor.h"
#include "../include/proposer.h"
#include "../include/learner.h"

unsigned int acceptor_id = 0;
unsigned int proposer_id = 0;
unsigned int learner_id = 0;
std::vector<std::thread*> thread_pool;

unsigned int acceptor_port = 0;
unsigned int port_number = 0;

std::vector<Component> proposers;
std::vector<Component> acceptors;
std::vector<Component> learners;

void create_acceptor()
{
    Acceptor acceptor = Acceptor(acceptor_id);
    acceptor.start_listening(port_number);
    // acceptor_id++;
}

void create_proposer()
{
    Proposer proposer = Proposer(proposer_id);
    sleep(1);
    proposer.prepare_request(proposer.proposal_number() % 10000, 20);
    proposer_id++;
}

void create_learner()
{
    Learner learner = Learner(learner_id);
    learner.start_listening(port_number);
    // learner_id++;
}

void start_acceptors(unsigned short n)
{
    for (int i = 0; i < n; i++) {
        std::thread* thread = new std::thread(create_acceptor);
        thread_pool.push_back(thread);
        sleep(5);
        std::cout << "N: " << n << std::endl;
    }
}

void start_proposers(unsigned short n)
{
    for (int i = 0; i < n; i++) {
        std::thread* thread = new std::thread(create_proposer);
        thread_pool.push_back(thread);
        sleep(1);
    }
}

void start_learners(unsigned short n)
{
    for (int i = 0; i < n; i++) {
        std::thread* thread = new std::thread(create_learner);
        thread_pool.push_back(thread);
        sleep(1);
    }
}

int main()
{   
    // proposers.push_back({PROPOSER, "127.0.0.1", 20100});
    // proposers.push_back({PROPOSER, "127.0.0.1", 20200});
    // proposers.push_back({PROPOSER, "127.0.0.1", 20300});

    // acceptors.push_back({ACCEPTOR, "127.0.0.1", 21100});
    // acceptors.push_back({ACCEPTOR, "127.0.0.1", 21200});
    // acceptors.push_back({ACCEPTOR, "127.0.0.1", 21300});
    // acceptors.push_back({ACCEPTOR, "127.0.0.1", 21400});

    // learners.push_back({LEARNER, "127.0.0.1", 22100});
    // learners.push_back({LEARNER, "127.0.0.1", 22200});

    // start_acceptors(acceptors.size());
    // start_learners(learners.size());
    // start_proposers(proposers.size());

    // for (std::thread* thread : thread_pool)
    //     thread->join();
    std::string id;
    std::cout << "Digite o id: ";
    std::cin >> id;
    std::cout << std::endl;

    std::string port;
    std::cout << "Digite a porta: ";
    std::cin >> port;
    std::cout << std::endl;

    proposer_id = stoi(id);
    port_number = stoi(port);

    std::thread t(create_proposer);
    t.join();

}