#ifndef AMETHYST_SERVER_CONNECTION_MANAGER_H
#define AMETHYST_SERVER_CONNECTION_MANAGER_H

/***********************************************************************
 Amethyst-GL
  - Connection Manager declarations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/


#include <set>
#include <boost/noncopyable.hpp>
#include "connection.h"

namespace amethyst {
namespace server {

/// Manages open connections so that they may be cleanly stopped when the server
/// needs to shut down.
class Connection_Manager
  : private boost::noncopyable
{
   public:
    /// Add the specified connection to the manager and start it.
    void start(TCP_Connection::sptr c);

    /// Stop the specified connection.
    void stop(TCP_Connection::sptr c);

    /// Stop all connections.
    void stop_all();

    /// Query number of active connections.
    int  get_number_of_connections();
    
    const std::set<TCP_Connection::sptr>::const_iterator iterator_begin();
    const std::set<TCP_Connection::sptr>::const_iterator iterator_end();
    
   private:
    /// The managed connections.
    std::set<TCP_Connection::sptr> connections_;
};

} // namespace server
} // namespace amethyst

#endif // AMETHYST_SERVER_CONNECTION_MANAGER_HPP
