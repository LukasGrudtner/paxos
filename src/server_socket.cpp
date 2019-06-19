#include "../include/server_socket.h"

ServerSocket::ServerSocket()
{
    m_work.reset(new asio::io_service::work(m_ios));
}

/* Start the server. */
void ServerSocket::start(unsigned short port_num, unsigned int thread_pool_size, PaxosComponent* component)
{
    assert(thread_pool_size > 0);

    /* Create and start Acceptor. */
    acc.reset(new Connection(m_ios, port_num, component));
    acc->start();

    /* Create specified number of threads and add them to the pool. */
    for (unsigned int i = 0; i < thread_pool_size; i++)
    {
        std::unique_ptr<std::thread> th(
            new std::thread([this]()
            {
                m_ios.run();
            }));
        
        m_thread_pool.push_back(std::move(th));
    }
}

/* Stops the server. */
void ServerSocket::stop()
{
    acc->stop();
    m_ios.stop();

    for (auto& th : m_thread_pool) {
        th->join();
    }
}