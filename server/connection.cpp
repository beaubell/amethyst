/***********************************************************************
 Amethyst-GL
  - Network Connection Class implementations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "connection.h"
#include "connection_manager.h"

namespace amethyst
{


void tcp_connection::start()
{
    std::cout << "Connection Started" << std::endl;

    message_ = "Welcome to the Amethyst Server.  Send me your GL Exts String now\r\n";
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
        boost::bind(&tcp_connection::handle_write, shared_from_this()));

    boost::asio::async_read_until(socket_, data_, "\r\n",
        boost::bind(&tcp_connection::handle_request_line, this, _1));

}

void tcp_connection::stop()
{
    std::cout << "Connection terminated" << std::endl;
    socket_.close();
}

void tcp_connection::handle_request_line(boost::system::error_code ec)
{
    if (!ec)
    {

        std::istream is(&data_);
        std::getline(is , gl_ext);

        std::cout << gl_ext << std::endl;

        boost::asio::async_read_until(socket_, data_, "\n",
            boost::bind(&tcp_connection::handle_request_line,
            shared_from_this(), boost::asio::placeholders::error));
    }
    else if (ec != boost::asio::error::operation_aborted)
    {
        connection_manager_.stop(shared_from_this());
    }
}

} // namespace amethyst
