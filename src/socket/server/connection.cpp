#include "../../../include/socket/server/connection.h"

Connection::Connection(asio::io_service& ios, unsigned short port_num, PaxosComponent* component) :
    m_ios(ios),
    m_acceptor(m_ios, asio::ip::tcp::endpoint(asio::ip::address_v4::any(), port_num)),
    m_is_stopped(false),
    px(component)
{}

/* Start accepting incoming connection requests. */
void Connection::start()
{
    std::cout << "Server started on " << m_acceptor.local_endpoint().address().to_string() << ":" << m_acceptor.local_endpoint().port() << std::endl;
    m_acceptor.listen();
    init_accept();
}

/* Stop accepting incoming connection requests. */
void Connection::stop()
{
    m_is_stopped.store(true);
}

void Connection::init_accept()
{
    std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(m_ios));

    m_acceptor.async_accept(*sock.get(),
        [this, sock](
            const boost::system::error_code& error)
            {
                on_accept(error, sock);
            });
}

void Connection::on_accept(const boost::system::error_code& ec, std::shared_ptr<asio::ip::tcp::socket> sock)
{
    if (ec == 0) {
        (new Service(sock, m_ios, px))->start_handling();
    }
    else {
        std::cout << "Error ocurred! Error code = " << ec.value() << ". Message: " << ec.message();
    }

    /* Init next async accept operation if acceptor has not been stopped yet. */
    if (!m_is_stopped.load()) {
        init_accept();
    }
    else {
        /* Stop accepting incoming connections and free allocated resources. */
        m_acceptor.close();
    }
}