#include "../include/proposer.h"

void handler(unsigned int request_id, const std::string& response, const system::error_code& ec)
{
    if (ec == 0) {
        std::cout << "Request #" << request_id << " has completed. Response: " << response << std::endl;
    } else if (ec == asio::error::operation_aborted) {
        std::cout << "Request #" << request_id << " has been cancelled by the user." << std::endl;
    } else {
        std::cout << "Request #" << request_id << " failed! Error code = " << ec.value() << ". Error message = " << ec.message() << std::endl;
    }
}

void Proposer::prepare_request()
{
    const std::string raw_ip_address = "127.0.0.1";


    try {
        ClientSocket socket;
        socket.send("prepare_request\n", 10, raw_ip_address, DEFAULT_PORT, handler, 1);

        std::cout << "send prepare" << std::endl;

        socket.close();
    }
    catch (system::system_error &e)
    {
        std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what();
    }
}