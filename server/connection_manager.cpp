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


void Connection_Manager::start(TCP_Connection::sptr c)
{
    connections_.insert(c);
    c->start();
}

void Connection_Manager::stop(TCP_Connection::sptr c)
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

int  Connection_Manager::get_number_of_connections()
{
    return connections_.size();
}

const std::set<TCP_Connection::sptr>::const_iterator Connection_Manager::iterator_begin()
{
    return connections_.begin();
}

const std::set<TCP_Connection::sptr>::const_iterator Connection_Manager::iterator_end()
{
    return connections_.end();
}

} // namespace server
} // namespace amethyst
