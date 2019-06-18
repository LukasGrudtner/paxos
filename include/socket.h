#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <boost/asio.hpp>

#include "connection.h"

using namespace boost;

class Socket
{
public:
    Socket();

    /* Starts the server, passing the port number and the number ofthreads to service requests. */
    void start(unsigned short port_num, unsigned int thread_pool_size);

    /* Stops the server, calling Acceptor stop() method. */
    void stop();

private:
    asio::io_service m_ios;
    std::unique_ptr<Connection> acc;
    std::unique_ptr<asio::io_service::work> m_work;
    std::vector<std::unique_ptr<std::thread>> m_thread_pool;
};

#endif