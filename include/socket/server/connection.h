#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <thread>
#include <atomic>
#include <memory>
#include <iostream>
#include <boost/asio.hpp>

#include "service.h"
#include "../../paxos_component.h"

using namespace boost;

class Connection
{
public:
    Connection(asio::io_service& ios, unsigned short port_num, PaxosComponent* component);

    /**
     * Instructs an object of the Acceptor class to start listening and accepting incoming connection
     * requests. It puts the m_acceptor acceptor socket into listening mode and then calls the class's 
     * init_accept() private method.
     **/
    void start();

    /**
     * Instructs the Acceptor object not to initiate the next asynchronous accept operation when
     * the currently running one completes.
     **/
    void stop();

private:
    /**
     * Constructs an active socket object and initiates the asynchronous accept operation, calling the
     * async_accept() method on the acceptor socket object.
     **/
    void init_accept();

    /* Callback method specified in the async_accept method. */
    void on_accept(const boost::system::error_code& ec, std::shared_ptr<asio::ip::tcp::socket> sock);

private:
    asio::io_service& m_ios;
    asio::ip::tcp::acceptor m_acceptor;
    std::atomic<bool> m_is_stopped;
    PaxosComponent* px;
};

#endif