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

namespace amethyst
{

/// Manages open connections so that they may be cleanly stopped when the server
/// needs to shut down.
class connection_manager
  : private boost::noncopyable
{
   public:
    /// Add the specified connection to the manager and start it.
    void start(connection_ptr c);

    /// Stop the specified connection.
    void stop(connection_ptr c);

    /// Stop all connections.
    void stop_all();

   private:
    /// The managed connections.
    std::set<connection_ptr> connections_;
};

} // namespace amethyst

#endif // AMETHYST_SERVER_CONNECTION_MANAGER_HPP
