#ifndef AMETHYST_CLIENT_NET_H
#define AMETHYST_CLIENT_NET_H

/***********************************************************************
 Amethyst-GL
  - Network setup and processing function prototypes

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include <sys/types.h>
#include <time.h>

#include "lib/object.h"
#include "lib/net.h"

#include "thread.h"

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "lib/manifest.h"

namespace amethyst {
namespace client {

class network_client
{
   public:
    network_client();
    void start();
    void stop();

   private:
    SF_Thread_Id net_thread;

};

int network_setup(void);

void net_send_telemetry(void);
void net_recv_telemetry(void);
int  net_start_thread();

#ifdef WIN32
#define GCC_PACK
#else
#define GCC_PACK __attribute__((__packed__))
#endif

#ifdef WIN32
#pragma pack(1)
#define u_int16_t unsigned short
#endif

struct packet_header
{
  // Sequence Counters
  u_int16_t  out_seq;
  u_int16_t  in_seq;

  // Time
  //u_int32_t  timestamp;

  // Packet Type
  u_int16_t  type;    // 1 == object transfer,  2 == chat?  maybe?
  u_int16_t  misc;    // if type==1,  misc = number of objects
} GCC_PACK;


struct object_transfer
{
  char                  name[12];
  lib::vectord_3d  location;
  lib::vectord_3d  velocity;
  lib::vectord_3d  acceleration;

  lib::vectord_4d  attitude;

  char        pad[12];
} GCC_PACK;

#ifdef WIN32
#pragma pack()
#endif

using namespace boost;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class Server_Connection
  //: public boost::enable_shared_from_this<server_connection>
{
   public:
    Server_Connection(lib::FileManifest& manifest)
        : io_service_(),
          socket_(io_service_),
          manifest_(manifest)
    {
    }

    void start(const std::string &server,
               const std::string &port,
               const std::string &user,
               const std::string &pass);

    void stop();

    void run(); //FIXME Migrate into start and make completely asynchryonous

   private:
    //  Read server header
    void handshake_read();
    void handle_handshake_read(boost::system::error_code ec);

    //  Send out header
    void handshake_send();
    void handle_handshake_send(boost::system::error_code ec);

    void login_read();
    void handle_login_read(boost::system::error_code ec);

    void login_send();
    void handle_login_send(boost::system::error_code ec);

    //void main_sequence_send();
    //void handle_main_sequence_send(boost::system::error_code ec);

    //void main_sequence_read();
    //void handle_main_sequence_read(boost::system::error_code ec);

    //void manifest_send();
    //void handle_manifest_send(boost::system::error_code ec);

    /// The io_service used to perform asynchronous operations.
    boost::asio::io_service io_service_;

    tcp::socket socket_;
    std::string message_;
    asio::streambuf in_data_;
    lib::FileManifest& manifest_;

    std::string server_prog;
    std::string server_version;
    std::string server_host;

    std::string server_user;
    std::string server_pass;
    std::string server_hash;
};

} // namespace client
} // namespace amethyst

#endif  /* NET_H */
