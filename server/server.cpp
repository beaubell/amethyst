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

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

  
Server::Server(const std::string& address, const std::string& port,
    const std::string& config_root)
  : io_service_(),
    acceptorV4_(io_service_),
    acceptorV6_(io_service_),
    connection_manager_(),
    new_connectionV4_(new TCP_Connection(io_service_, connection_manager_, manifest_)),
    new_connectionV6_(new TCP_Connection(io_service_, connection_manager_, manifest_)),
    file_root(config_root)
{
    // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
    tcp::resolver resolver(io_service_);
    tcp::resolver::query query(address, port);
    tcp::endpoint endpoint = *resolver.resolve(query);
    
    //acceptor_.open(endpoint.protocol());
    acceptorV4_.open(tcp::v4());
    acceptorV4_.set_option(tcp::acceptor::reuse_address(true));
    acceptorV4_.bind(endpoint);
    //acceptor_.bind(tcp::endpoint(boost::asio::ip::tcp::v4(), port));
    acceptorV4_.listen();
    acceptorV4_.async_accept(new_connectionV4_->socket(),
      boost::bind(&Server::handle_acceptV4, this, boost::asio::placeholders::error));

    acceptorV6_.open(tcp::v6());
    acceptorV6_.set_option(tcp::acceptor::reuse_address(true));
    acceptorV6_.bind(endpoint);
    //acceptor_.bind(tcp::endpoint(boost::asio::ip::tcp::v4(), port));
    acceptorV6_.listen();
    acceptorV6_.async_accept(new_connectionV6_->socket(),
      boost::bind(&Server::handle_acceptV6, this, boost::asio::placeholders::error));
      
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

void Server::handle_acceptV4(const boost::system::error_code& e)
{
  if (!e)
  {
    connection_manager_.start(new_connectionV4_);
    new_connectionV4_.reset(new TCP_Connection(io_service_, connection_manager_, manifest_));
    acceptorV4_.async_accept(new_connectionV4_->socket(),
        boost::bind(&Server::handle_acceptV4, this, boost::asio::placeholders::error));
  }
}

void Server::handle_acceptV6(const boost::system::error_code& e)
{
  if (!e)
  {
    connection_manager_.start(new_connectionV6_);
    new_connectionV6_.reset(new TCP_Connection(io_service_, connection_manager_, manifest_));
    acceptorV6_.async_accept(new_connectionV6_->socket(),
        boost::bind(&Server::handle_acceptV6, this, boost::asio::placeholders::error));
  }
}

void Server::handle_stop()
{
  // The server is stopped by cancelling all outstanding asynchronous
  // operations. Once all operations have finished the io_service::run() call
  // will exit.
  acceptorV4_.close();
  acceptorV6_.close();
  connection_manager_.stop_all();
}


} // namespace server
} // namespace amethyst
