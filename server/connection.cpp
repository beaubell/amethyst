/***********************************************************************
 Amethyst-Server
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

    // Send handshake
    handshake_send();
}

void tcp_connection::stop()
{
    std::cout << "Connection terminated" << std::endl;
    socket_.close();
}

void tcp_connection::handshake_send()
{
    // Write Header
    message_ = "Amethyst-Server 0.1 %host\r\n";
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
      boost::bind(&tcp_connection::handle_handshake_send, shared_from_this(), _1));
}

void tcp_connection::handle_handshake_send(boost::system::error_code ec)
{
    if (!ec)
    {
        // Header went out fine, now wait for client to send header.
        boost::asio::async_read_until(socket_, in_data_, "\r\n",
         boost::bind(&tcp_connection::handle_handshake_read, shared_from_this(), _1));
    }
    else if (ec != boost::asio::error::operation_aborted)
    {
        connection_manager_.stop(shared_from_this());
    }
}

void tcp_connection::handle_handshake_read(boost::system::error_code ec)
{
    if (!ec)
    {
        std::istream is(&in_data_);

        // Check header
        is >> client_prog >> client_version >> client_host;
        is.ignore();

        // Check version
        if(!(client_version == "0.1"))
        {
            // You need to upgrade
            message_ = "New Version: 0.1 Upgrade now, bitch.\r\n";
            boost::asio::async_write(socket_, boost::asio::buffer(message_),
             boost::bind(&tcp_connection::stop, shared_from_this()));
            return;
        }

        // Now proceed to the login section;
        login_send();
    }
    else if (ec != boost::asio::error::operation_aborted)
    {
        connection_manager_.stop(shared_from_this());
    }
}

void tcp_connection::login_send()
{
    // Generate hash
    //client_hash = md5sum(
    boost::hash<std::string> string_hash;

    client_hash = string_hash("HAHAHAHSHSHDHDHSHS");

    message_ =  "login " + client_hash + "\r\n";

    boost::asio::async_write(socket_, boost::asio::buffer(message_),
     boost::bind(&tcp_connection::handle_login_send, shared_from_this(), _1));
}

void tcp_connection::handle_login_send(boost::system::error_code ec)
{
    if (!ec)
    {
        // ok, now wait for login reply
        login_read();
    }
    else if (ec != boost::asio::error::operation_aborted)
    {
        connection_manager_.stop(shared_from_this());
    }
}

void tcp_connection::login_read()
{
    boost::asio::async_read_until(socket_, in_data_, "\r\n",
     boost::bind(&tcp_connection::handle_login_read, shared_from_this(), _1));

}

void tcp_connection::handle_login_read(boost::system::error_code ec)
{
    if (!ec)
    {
        std::istream is(&in_data_);

        // Is login valid?
        is >> client_user >> client_pass;
        is.ignore(255);

        // check user and pass agianst database;
        if(1)
        {
            main_sequence_send();
            return;
        }

        // No
        client_login_attempts++;
        std::cout << client_login_attempts << std::endl;
        if (client_login_attempts > 2)
        {
            // Too many login attempts
            message_ = "Error: Login attempts exceeded.\r\n";
            boost::asio::async_write(socket_, boost::asio::buffer(message_),
             boost::bind(&tcp_connection::stop, shared_from_this()));
            return;
        }
        else
        {
           login_send();
           return;
        }
    }
    else if (ec != boost::asio::error::operation_aborted)
    {
        connection_manager_.stop(shared_from_this());
    }
}


void tcp_connection::main_sequence_send()
{
    message_ = "Ok. Client Ready...\r\n";
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
     boost::bind(&tcp_connection::handle_main_sequence_send, shared_from_this(), _1));

}

void tcp_connection::handle_main_sequence_send(boost::system::error_code ec)
{
if (!ec)
    {
        main_sequence_read();
    }
    else if (ec != boost::asio::error::operation_aborted)
    {
        connection_manager_.stop(shared_from_this());
    }


}

void tcp_connection::main_sequence_read()
{
    boost::asio::async_read_until(socket_, in_data_, "\r\n",
     boost::bind(&tcp_connection::handle_main_sequence_read, shared_from_this(), _1));
}

void tcp_connection::handle_main_sequence_read(boost::system::error_code ec)
{
    if (!ec)
    {
        std::istream is(&in_data_);

        std::string command;
        is >> command;

        is.ignore(255);

        std::cout << "Got Command: " << command << std::endl;

        // FIXME Handle commands
        if (command == "manifest")
        {}
        else if (command == "get")
        {}


        boost::asio::async_read_until(socket_, in_data_, "\r\n",
         boost::bind(&tcp_connection::handle_main_sequence_read, shared_from_this(), _1));

    }
    else if (ec != boost::asio::error::operation_aborted)
    {
        connection_manager_.stop(shared_from_this());
    }
}


} // namespace amethyst
