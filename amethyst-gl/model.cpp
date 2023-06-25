/***********************************************************************
 Amethyst-GL
  - Model loading and processing function implementations

 Authors (c):
 2004-2023 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "model.h"

namespace amethyst::client {


void Model::render(const TransMatrix& m_proj, const TransMatrix& m_view, const TransMatrix& m_model) const
{
    using namespace std::placeholders;

    for(auto& prim: _primitives) {
        prim->render(m_proj, m_view, m_model);
    }
}

void Model::addPrimitive(Primitive::sptr newprim)
{
    _primitives.push_back(std::move(newprim));
}

const std::list<Primitive::sptr>& Model::getPrimitives()
{
    return _primitives;
}


} // namespace amethyst::client
