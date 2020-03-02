#pragma once

/***********************************************************************
 Amethyst-GL
  - Model loading and processing function prototypes

 Authors (c):
 2004-2020 Beau V.C. Bellamy (beau@stellarnetservices.net)
 ***********************************************************************/

#include "texture.h"
#include "primitive.h"

#include "lib/types.h"
#include "lib/rendermodel.h"
#include "lib/resource.h"
#include "shaderprog_model.h"

#include <string>
#include <memory>

namespace amethyst {
namespace client {

class Model : public lib::RenderModel
{
   public:
    typedef std::shared_ptr<Model> sptr;
    typedef std::weak_ptr<Model>   wptr;

    Model();
    virtual ~Model();
    virtual void render(const TransMatrix& m_proj, const TransMatrix& m_view, const TransMatrix& m_model);

    void addPrimitive(Primitive::sptr newprim);
    const std::list<Primitive::sptr>& getPrimitives();

  private:
    std::list<Primitive::sptr> _primitives;
};

} // namespace client
} // namsspace amethyst
