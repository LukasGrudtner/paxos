#include "../include/acceptor.h"
#include "../include/proposer.h"

void start_acceptor()
{
    Acceptor acceptor;
    acceptor.start_listening();
}

void start_proposer()
{
    Proposer proposer;
    proposer.prepare_request();
}

int main()
{

    std::thread t1(start_acceptor);
    sleep(1);
    std::thread t2(start_proposer);
    t1.join();
    t2.join();
    
}