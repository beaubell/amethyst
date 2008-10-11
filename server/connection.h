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
    typedef boost::shared_ptr<tcp_connection> pointer;

    //static pointer create(boost::asio::io_service& io_service)
    //{
    //    return pointer(new tcp_connection(io_service));
    //}

    tcp::socket& socket()
    {
        return socket_;
    }

    void start();

    void stop();

    void handle_request_line(boost::system::error_code ec);

   //private:
    tcp_connection(boost::asio::io_service& io_service, connection_manager& manager)
       : socket_(io_service), connection_manager_(manager)
    {
    }

    void handle_write()
    {
    }

    tcp::socket socket_;
    std::string message_;
    std::string gl_ext;
    asio::streambuf data_;
    connection_manager& connection_manager_;
};

typedef boost::shared_ptr<tcp_connection> connection_ptr;

} // Namespace amethyst

#endif
