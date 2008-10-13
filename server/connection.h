#ifndef AMETHYST_SERVER_CONNECTION_H
#define AMETHYST_SERVER_CONNECTION_H

/***********************************************************************
 Amethyst-Server
  - Network Connection Class declarations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "lib/manifest.h"

using namespace boost;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;

namespace amethyst
{

class connection_manager;


class tcp_connection
  : public boost::enable_shared_from_this<tcp_connection>
{
   public:
    tcp_connection(boost::asio::io_service& io_service,
                   connection_manager& manager,
                   lib::filemanifest& manifest)
       : socket_(io_service),
         connection_manager_(manager),
         manifest_(manifest),
         client_login_attempts(0)
    {
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    void start();

    void stop();

   private:
    //  Check to see if handshake was sent out properly
    void handshake_send();
    void handle_handshake_send(boost::system::error_code ec);

    //  Handle to see if client sent proper data
    void handshake_read();
    void handle_handshake_read(boost::system::error_code ec);
    //
    void login_send();
    void handle_login_send(boost::system::error_code ec);

    void login_read();
    void handle_login_read(boost::system::error_code ec);

    void main_sequence_send();
    void handle_main_sequence_send(boost::system::error_code ec);

    void main_sequence_read();
    void handle_main_sequence_read(boost::system::error_code ec);

    void manifest_send();
    void handle_manifest_send(boost::system::error_code ec);

    tcp::socket socket_;
    std::string message_;
    asio::streambuf in_data_;
    connection_manager& connection_manager_;
    lib::filemanifest& manifest_;

    std::string client_prog;
    std::string client_version;
    std::string client_host;
    std::string client_user;
    std::string client_pass;
    std::string client_hash;
    std::string client_glext;

    short client_login_attempts;
};

typedef boost::shared_ptr<tcp_connection> connection_ptr;

} // Namespace amethyst

#endif
