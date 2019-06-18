#include "../include/acceptor.h"

Acceptor::Acceptor()
{
}

void Acceptor::start_listening()
{
    try {
        ServerSocket socket;
        socket.start(DEFAULT_PORT, DEFAULT_THREAD_POOL_SIZE);

        while (1) std::this_thread::sleep_for(std::chrono::seconds(60));
        socket.stop();
    }
    catch (system::system_error& e) {
        std::cout << "Error ocurred! Error code = " << e.code() << ". Message: " << e.what();
    }
}