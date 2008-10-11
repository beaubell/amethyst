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

namespace amethyst
{



class server
  : private boost::noncopyable
{
   public:
    /// Construct the server to listen on the specified TCP address and port, and
    /// serve up files from the given directory.
    explicit server(const std::string& address, const std::string& port,
        const std::string& file_root);

    /// Run the server's io_service loop.
    void run();

    /// Stop the server.
    void stop();

   private:
    /// Handle completion of an asynchronous accept operation.
    void handle_accept(const boost::system::error_code& e);

    /// Handle a request to stop the server.
    void handle_stop();

    /// The io_service used to perform asynchronous operations.
    boost::asio::io_service io_service_;

    /// Acceptor used to listen for incoming connections.
    boost::asio::ip::tcp::acceptor acceptor_;

    /// The connection manager which owns all live connections.
    connection_manager connection_manager_;

    /// The next connection to be accepted.
    connection_ptr new_connection_;

    /// The handler for all incoming requests.
    //request_handler request_handler_;
};

} // namespace amethyst
#endif
