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
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "lib/manifest.h"

#include <memory>

using namespace boost;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;

namespace amethyst {
namespace server {

class Connection_Manager;


class TCP_Connection
  : public std::enable_shared_from_this<TCP_Connection>
{
   public:
    typedef std::shared_ptr<TCP_Connection> sptr;

    TCP_Connection(boost::asio::io_service& io_service,
                   Connection_Manager& manager,
                   lib::FileManifest& manifest)
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
    
    void connectionlist_send();
    void handle_connectionlist_send(boost::system::error_code ec);

    tcp::socket socket_;
    std::string message_;
    asio::streambuf in_data_;
    Connection_Manager& connection_manager_;
    lib::FileManifest& manifest_;

    std::string client_prog;
    std::string client_version;
    std::string client_host;
    std::string client_user;
    std::string client_pass;
    std::string client_hash;
    std::string client_glext;

    short client_login_attempts;
};

} // namespace server
} // Namespace amethyst

#endif
