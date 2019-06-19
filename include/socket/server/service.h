#ifndef __SERVICE_H__
#define __SERVICE_H__

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "../../paxos_component.h"

using namespace boost;

/**
 * One instance of this class is intended to handle a single connected client by reading the request,
 * processing it, and then sending back the response message.
 **/
class Service
{
public:
    Service(std::shared_ptr<asio::ip::tcp::socket> sock, asio::io_service& io, PaxosComponent* component);

    /**
     * Starts handling the client by initiating the asynchronous reading operation to read 
     * the request message from the client specifying the on_request_received() method as a callback.
     **/
    void start_handling();

private:
    /* Called when the connection deadline timer expires. */
    void connection_timeout(const boost::system::error_code& ec);

    /**
     * When the request reading completes, or an error occurs, the callback method on_request_received()
     * is called. If there were no errors, call the process_request() method to process de client
     * request. Once answered, call start_handling() again handle with more requests from the same client.
     */
    void on_requested_received(const boost::system::error_code& ec, std::size_t bytes_transferred);

    void on_response_sent(const boost::system::error_code& ec, std::size_t bytes_transferred);

    /* Delete this object when its job is completed. */
    void on_finish();

    /* Processes the client requests, writing the received data in files. */
    void process_request(asio::streambuf& request);

private:
    std::string m_response;
    asio::streambuf m_request;
    std::shared_ptr<asio::ip::tcp::socket> m_sock;
    asio::io_service& m_ios;
    PaxosComponent* px;
};

#endif