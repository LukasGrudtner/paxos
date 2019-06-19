#include "../include/client_socket.h"

ClientSocket::ClientSocket()
{
    m_work.reset(new boost::asio::io_service::work(m_ios));
    m_thread.reset(new std::thread([this](){
        m_ios.run();
    }));
}

void ClientSocket::send(std::string request, unsigned int duration_sec, const std::string& raw_ip_address, unsigned short port_num, PaxosComponent* component, unsigned int request_id)
{
    std::shared_ptr<Session> session =
        std::shared_ptr<Session>(new Session(m_ios,
            raw_ip_address,
            port_num,
            request,
            request_id,
            component));
    
    session->m_sock.open(session->m_ep.protocol());

    /**
     * Add new session to the list of active sessions so that we can access it if the user
     * decides to cancel the coresponding request before it completes.
     * Because active sessions list can be accessed from multiple thread, we guard it with
     * a mutex to avoit data corruption.
     */
          std::unique_lock<std::mutex>
         lock(m_active_sessions_guard);
      m_active_sessions[request_id] = session;
      lock.unlock();

      session->m_sock.async_connect(session->m_ep, 
         [this, session](const system::error_code& ec) 
         {
         if (ec != 0) {
            session->m_ec = ec;
            on_request_complete(session);
            return;
         }

         std::unique_lock<std::mutex>
            cancel_lock(session->m_cancel_guard);

         if (session->m_was_cancelled) {
            on_request_complete(session);
            return;
         }

                asio::async_write(session->m_sock, 
                             asio::buffer(session->m_request),
         [this, session](const boost::system::error_code& ec,
                            std::size_t bytes_transferred) 
         {
         if (ec != 0) {
            session->m_ec = ec;
            on_request_complete(session);
            return;
         }


         std::unique_lock<std::mutex>
            cancel_lock(session->m_cancel_guard);

         if (session->m_was_cancelled) {
            on_request_complete(session);
            return;
         }

        asio::async_read_until(session->m_sock,
                                  session->m_response_buf, 
                                  '\n', 
         [this, session](const boost::system::error_code& ec,
              std::size_t bytes_transferred) 
         {
         if (ec != 0) {
            session->m_ec = ec;
         } else {
            std::istream strm(&session->m_response_buf);
            std::getline(strm, session->m_response);
         }

         on_request_complete(session);
      });});});
}

void ClientSocket::cancel_request(unsigned int request_id)
{
    std::unique_lock<std::mutex> lock(m_active_sessions_guard);

    auto it = m_active_sessions.find(request_id);
    if (it != m_active_sessions.end()) {
        std::unique_lock<std::mutex> cancel_lock(it->second->m_cancel_guard);

        it->second->m_was_cancelled = true;
        it->second->m_sock.cancel();
    }
}

void ClientSocket::close()
{
    /**
     * Destroy work object. This allows the I/O thread to exist the event loop
     * when there are no more peding asynchronous operations.
     */
    m_work.reset(NULL);

    /* Wait for the I/O thread to exist. */
    m_thread->join();
}

void ClientSocket::on_request_complete(std::shared_ptr<Session> session)
{
    /**
     * Shutting down the connection. This method may fail in case socket is not connected.
     * We don't care about the error code if this function fails.
     */
    boost::system::error_code ignored_ec;

    session->m_sock.shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);

    /* Remove session form the map of active sessions. */
    std::unique_lock<std::mutex> lock(m_active_sessions_guard);

    auto it = m_active_sessions.find(session->m_id);
    if (it != m_active_sessions.end())
        m_active_sessions.erase(it);

    lock.unlock();

    boost::system::error_code ec;

    if (session->m_ec == 0 && session->m_was_cancelled)
        ec = asio::error::operation_aborted;
    else
        ec = session->m_ec;
    
    /* Call the callback provided by the user. */
    session->m_component->on_received_response(session->m_response);
}