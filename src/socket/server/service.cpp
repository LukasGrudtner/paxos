#include "../../../include/socket/server/service.h"

Service::Service(std::shared_ptr<asio::ip::tcp::socket> sock, asio::io_service& io, PaxosComponent* component) : 
        m_sock(sock), 
        m_ios(io),
        px(component)
{}


void Service::connection_timeout(const boost::system::error_code& ec)
{
    /*
        If we arrived here without the timer expired, ignore it.
        If not, close the connection with this client.
    */

    if (ec != boost::asio::error::operation_aborted) {
        on_finish();
    }
}

void Service::start_handling()
{
    asio::async_read_until(*m_sock.get(),
        m_request,
        '\n',
        [this](
            const boost::system::error_code& ec,
            std::size_t bytes_transferred)
            {
                on_requested_received(ec, bytes_transferred);
            }
        );
}

void Service::on_requested_received(const boost::system::error_code& ec, std::size_t bytes_transferred)
{
    if (ec != 0) {
        on_finish();
        return;
    }

    /* Process the request. */
    process_request(m_request);

    /* Clear the buffer and call start_handling again. */ 
    m_request.consume(m_request.size());
    start_handling();
}

void Service::on_response_sent(const boost::system::error_code& ec, std::size_t bytes_transferred)
{
    if (ec != 0) {
        std::cout << "Error ocurred! Error code = " << ec.value() << ". Message: " << ec.message();
    }

    on_finish();
}

/* Here we perform the cleanup. */
void Service::on_finish()
{
    delete this;
}

void Service::process_request(asio::streambuf& request)
{
    /* In this method we parse the request, process it and prepare the request. */
    /* Prepare writing of received data. */
    std::string response(boost::asio::buffers_begin(request.data()), boost::asio::buffers_begin(request.data()) + request.size());

    std::string m_response = px->on_received_response(response);

    asio::async_write(*m_sock.get(), 
            asio::buffer(m_response),
            [this](
                            const boost::system::error_code& ec,
                            std::size_t bytes_transferred) 
                            {
                                on_response_sent(ec,
                                  bytes_transferred);
               });

}