#ifndef AMETHYST_SERVER_SERVER_H
#define AMETHYST_SERVER_SERVER_H

/***********************************************************************
 Amethyst-Server
  - Server Class declarations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include <boost/asio.hpp>
#include <string>
#include <boost/noncopyable.hpp>
#include "connection.h"
#include "connection_manager.h"
#include "manifest.h"

namespace amethyst {
namespace server {


class Server
  : private boost::noncopyable
{
   public:
    /// Construct the server to listen on the specified TCP address and port, and
    /// serve up files from the given directory.
    explicit Server(const std::string& address, const std::string& port,
        const std::string& file_root);

    /// Run the server's io_service loop.
    void run();

    /// Stop the server.
    void stop();

   private:
    /// Handle completion of an asynchronous accept operation.
    void handle_acceptV4(const boost::system::error_code& e);
    void handle_acceptV6(const boost::system::error_code& e);

    /// Handle a request to stop the server.
    void handle_stop();

    /// The io_service used to perform asynchronous operations.
    boost::asio::io_service io_service_;

    /// Acceptor used to listen for incoming connections.
    boost::asio::ip::tcp::acceptor acceptorV4_;
    boost::asio::ip::tcp::acceptor acceptorV6_;

    /// The connection manager which owns all live connections.
    Connection_Manager connection_manager_;

    /// The next connection to be accepted.
    TCP_Connection::sptr new_connectionV4_;
    TCP_Connection::sptr new_connectionV6_;

    /// The handler for all incoming requests.
    //request_handler request_handler_;

    /// File Root
    std::string file_root;
    /// Manifest
    lib::FileManifest manifest_;
};

} // namespace server
} // namespace amethyst

#endif // AMETHYST_SERVER_SERVER_H
