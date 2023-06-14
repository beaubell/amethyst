#ifndef AMETHYST_SERVER_CONNECTION_MANAGER_H
#define AMETHYST_SERVER_CONNECTION_MANAGER_H

/***********************************************************************
 Amethyst-GL
  - Connection Manager declarations

 Authors (c):
 2006-2023 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/


#include <set>
#include <boost/noncopyable.hpp>
#include "connection.h"

namespace amethyst::server {

/// Manages open connections so that they may be cleanly stopped when the server
/// needs to shut down.
class Connection_Manager
  : private boost::noncopyable
{
   public:
    /// Add the specified connection to the manager and start it.
    void start(const TCP_Connection::sptr& c);

    /// Stop the specified connection.
    void stop(const TCP_Connection::sptr& c);

    /// Stop all connections.
    void stop_all();

    /// Query number of active connections.
    [[nodiscard]] size_t  get_number_of_connections() const;
    
    std::set<TCP_Connection::sptr>::const_iterator iterator_begin();
    std::set<TCP_Connection::sptr>::const_iterator iterator_end();
    
   private:
    /// The managed connections.
    std::set<TCP_Connection::sptr> connections_;
};

} // namespace amethyst::server

#endif // AMETHYST_SERVER_CONNECTION_MANAGER_H
