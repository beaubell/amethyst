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
#include <boost/crypto/sha2.hpp>
#include <boost/crypto/message_digest.hpp>
#include "boost/date_time/local_time/local_time.hpp"


namespace amethyst {
namespace server {


void TCP_Connection::start()
{
    std::cout << "Connection Started" << std::endl;

    // Send handshake
    handshake_send();
}

void TCP_Connection::stop()
{
    std::cout << "Connection terminated" << std::endl;
    socket_.close();
}

void TCP_Connection::handshake_send()
{
    // Write Header
    message_ = "Amethyst-Server 0.0.1 %host\r\n";
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
      boost::bind(&TCP_Connection::handle_handshake_send, shared_from_this(), _1));
}

void TCP_Connection::handle_handshake_send(boost::system::error_code ec)
{
    if (!ec)
    {
        // Header went out fine, now wait for client to send header.
        boost::asio::async_read_until(socket_, in_data_, "\r\n",
         boost::bind(&TCP_Connection::handle_handshake_read, shared_from_this(), _1));
    }
    else if (ec != boost::asio::error::operation_aborted)
    {
        connection_manager_.stop(shared_from_this());
    }
}

void TCP_Connection::handle_handshake_read(boost::system::error_code ec)
{
    if (!ec)
    {
        std::istream is(&in_data_);

        // Check header
        is >> client_prog >> client_version >> client_host;
        is.ignore();

        // Check version
        if(!(client_version == "0.0.1"))
        {
            // You need to upgrade
            message_ = "NewVersion: 0.0.1\r\n";
            boost::asio::async_write(socket_, boost::asio::buffer(message_),
             boost::bind(&TCP_Connection::stop, shared_from_this()));
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

void TCP_Connection::login_send()
{
    // Generate hash
    using namespace boost::gregorian;
    using namespace boost::local_time;
    using namespace boost::posix_time;
    boost::crypto::message_digest<boost::crypto::detail::sha256_ctx> sha256;

    // Get localtime then get sha256sum of it
    ptime t(second_clock::local_time());
    sha256.input(to_simple_string(t));
    client_hash = sha256.to_string();

    // Construct header
    message_ =  "login " + client_hash + "\r\n";

    boost::asio::async_write(socket_, boost::asio::buffer(message_),
     boost::bind(&TCP_Connection::handle_login_send, shared_from_this(), _1));
}

void TCP_Connection::handle_login_send(boost::system::error_code ec)
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

void TCP_Connection::login_read()
{
    boost::asio::async_read_until(socket_, in_data_, "\r\n",
     boost::bind(&TCP_Connection::handle_login_read, shared_from_this(), _1));

}

void TCP_Connection::handle_login_read(boost::system::error_code ec)
{
    if (!ec)
    {
        std::istream is(&in_data_);

        // Is login valid?
        is >> client_user >> client_pass;
        is.ignore(255);

        std::cout << "Received Login info: " << client_user << " " << client_pass << std::endl;

        boost::crypto::message_digest<boost::crypto::detail::sha256_ctx> sha256;

        sha256.input("test"+client_hash);
        std::string hash = sha256.to_string();

        if (hash == client_pass)
            std::cout << "Login Sucessful!!" << std::endl;
        else
            std::cout << "Login Not Sucessful!!" << std::endl;

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
             boost::bind(&TCP_Connection::stop, shared_from_this()));
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


void TCP_Connection::main_sequence_send()
{
    message_ = "Ok. Client Ready...\r\n";
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
     boost::bind(&TCP_Connection::handle_main_sequence_send, shared_from_this(), _1));

}

void TCP_Connection::handle_main_sequence_send(boost::system::error_code ec)
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

void TCP_Connection::main_sequence_read()
{
    boost::asio::async_read_until(socket_, in_data_, "\r\n",
     boost::bind(&TCP_Connection::handle_main_sequence_read, shared_from_this(), _1));
}

void TCP_Connection::handle_main_sequence_read(boost::system::error_code ec)
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
        {
            manifest_send();
        }
        else if (command == "get")
        {}
        else if (command == "bye")
        {
            connection_manager_.stop(shared_from_this());
        }

        boost::asio::async_read_until(socket_, in_data_, "\r\n",
         boost::bind(&TCP_Connection::handle_main_sequence_read, shared_from_this(), _1));

    }
    else if (ec != boost::asio::error::operation_aborted)
    {
        connection_manager_.stop(shared_from_this());
    }
}

void TCP_Connection::manifest_send()
{
    message_ = "Ok. " + lexical_cast<std::string>(manifest_.size()) + "Entries\r\n";
    for (int count = 0;count < manifest_.size();count++)
        message_ += manifest_[count].hash + " " + manifest_[count].file
                 + " " + lexical_cast<std::string>(manifest_[count].size) + "\r\n";

    message_ += "Ok. Client Ready...\r\n";
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
     boost::bind(&TCP_Connection::handle_main_sequence_send, shared_from_this(), _1));

}

} // namespace server
} // namespace amethyst
