/***********************************************************************
 Amethyst-Server
  - Server Class implementations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "server.h"
#include <boost/bind.hpp>

namespace amethyst {
namespace server {

Server::Server(const std::string& address, const std::string& port,
    const std::string& config_root)
  : io_service_(),
    acceptor_(io_service_),
    connection_manager_(),
    new_connection_(new TCP_Connection(io_service_, connection_manager_, manifest_)),
    file_root(config_root)
{
    // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
    boost::asio::ip::tcp::resolver resolver(io_service_);
    boost::asio::ip::tcp::resolver::query query(address, port);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
    //acceptor_.open(endpoint.protocol());
    acceptor_.open(boost::asio::ip::tcp::v4());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    //acceptor_.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
    acceptor_.listen();
    acceptor_.async_accept(new_connection_->socket(),
      boost::bind(&Server::handle_accept, this,
      boost::asio::placeholders::error));

    // Get file manifest
    manifest_.initialize(config_root);
}

void Server::run()
{
    // The io_service::run() call will block until all asynchronous operations
    // have finished. While the server is running, there is always at least one
    // asynchronous operation outstanding: the asynchronous accept call waiting
    // for new incoming connections.
    io_service_.run();
}

void Server::stop()
{
  // Post a call to the stop function so that server::stop() is safe to call
  // from any thread.
  io_service_.post(boost::bind(&Server::handle_stop, this));
}

void Server::handle_accept(const boost::system::error_code& e)
{
  if (!e)
  {
    connection_manager_.start(new_connection_);
    new_connection_.reset(new TCP_Connection(io_service_, connection_manager_, manifest_));
    acceptor_.async_accept(new_connection_->socket(),
        boost::bind(&Server::handle_accept, this,
          boost::asio::placeholders::error));
  }
}

void Server::handle_stop()
{
  // The server is stopped by cancelling all outstanding asynchronous
  // operations. Once all operations have finished the io_service::run() call
  // will exit.
  acceptor_.close();
  connection_manager_.stop_all();
}


} // namespace server
} // namespace amethyst
