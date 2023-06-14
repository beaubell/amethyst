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

namespace amethyst::server {


void Connection_Manager::start(const TCP_Connection::sptr& c)
{
    connections_.insert(c);
    c->start();
}

void Connection_Manager::stop(const TCP_Connection::sptr& c)
{
    connections_.erase(c);
    c->stop();
}

void Connection_Manager::stop_all()
{
    std::for_each(connections_.begin(), connections_.end(),
       [](const auto& conn){ conn->stop(); });
    connections_.clear();
}

size_t Connection_Manager::get_number_of_connections() const
{
    return connections_.size();
}

std::set<TCP_Connection::sptr>::const_iterator Connection_Manager::iterator_begin()
{
    return connections_.cbegin();
}

std::set<TCP_Connection::sptr>::const_iterator Connection_Manager::iterator_end()
{
    return connections_.cend();
}

} // namespace amethyst::server
