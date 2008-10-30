/***********************************************************************
 Amethyst-GL
  - Connection Manager implementations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "connection_manager.h"
#include <algorithm>
#include <boost/bind.hpp>

namespace amethyst {
namespace server {


void Connection_Manager::start(connection_ptr c)
{
    connections_.insert(c);
    c->start();
}

void Connection_Manager::stop(connection_ptr c)
{
    connections_.erase(c);
    c->stop();
}

void Connection_Manager::stop_all()
{
    std::for_each(connections_.begin(), connections_.end(),
       boost::bind(&TCP_Connection::stop, _1));
    connections_.clear();
}

} // namespace server
} // namespace amethyst
