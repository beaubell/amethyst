/***********************************************************************
 Amethyst-GL
  - Model loading and processing function implementations

 Authors (c):
 2004-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "model.h"

#include <string>    // std::string
#include <functional>

namespace amethyst {
namespace client {

std::unordered_map<std::string, Model::sptr>  model_list;

//
//
Model::Model()
{}

Model::~Model()
{

}

void Model::render(const TransMatrix& m_proj, const TransMatrix& m_view, const TransMatrix& m_model)
{
    using namespace std::placeholders;

    std::for_each(_primitives.begin(), _primitives.end(),
       std::bind(&Primitive::render, _1, m_proj, m_view, m_model));
}

void Model::addPrimitive(Primitive::sptr newprim)
{
    _primitives.push_back(newprim);
}

const std::list<Primitive::sptr>& Model::getPrimitives()
{
    return _primitives;
}



} // namespace client
} // namespace amethyst
