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


void connection_manager::start(connection_ptr c)
{
    connections_.insert(c);
    c->start();
}

void connection_manager::stop(connection_ptr c)
{
    connections_.erase(c);
    c->stop();
}

void connection_manager::stop_all()
{
    std::for_each(connections_.begin(), connections_.end(),
       boost::bind(&tcp_connection::stop, _1));
    connections_.clear();
}


} // namespace amethyst
