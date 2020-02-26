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
#include <list>
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
    //virtual void clear();

    //void setName(const std::string& name);
    //const std::string& getName();

    void addPrimitive(Primitive::sptr newprim);
    const std::list<Primitive::sptr>& getPrimitives();

  private:
    std::list<Primitive::sptr> _primitives;
};

extern std::list<Model::sptr>  model_list;

void model_add(Model::sptr newmodel);
Model::sptr model_find(const std::string &name);
void models_free(void);

Model::sptr model_load(std::string &model_name);


void model_load_file(const Resource& res, Model &model, Texture::sptr tex, ShaderProgramModel::sptr shdr);

TriangleStrip::sptr model_sphere_create(double cx, double cy, double cz, double r, int p );

} // namespace client
} // namsspace amethyst
