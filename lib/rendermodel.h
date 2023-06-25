#ifndef AMETHYST_LIB_RENDERMODEL_H
#define AMETHYST_LIB_RENDERMODEL_H

/***********************************************************************
 Amethyst-lib
  - Rendering Model Base Class, Declarations

 Authors (c):
 2016 Beau V.C. Bellamy (bellamy.beau@gmail.com)

 ***********************************************************************/

#include "types.h" // For TransMatrix

#include <memory>  // For shared_ptr, weak_ptr
#include <string>  // For sring

namespace amethyst::lib {

class RenderModel
{
  public:
    typedef std::shared_ptr<RenderModel> sptr;
    typedef std::weak_ptr<RenderModel>   wptr;

    RenderModel() = default;
    explicit RenderModel(const std::string& new_name) : _name(new_name) {};
    explicit RenderModel(std::string&& new_name) : _name(std::move(new_name)) {};
    virtual ~RenderModel() = default;
    virtual void render(const TransMatrix& m_proj, const TransMatrix& m_view, const TransMatrix& m_model) const = 0;
    //virtual void clear();

    void setName(const std::string& name);
    const std::string& getName();

  private:
    std::string _name;
    //std::list<Primative::sptr> _primatives;
};

} // namespace amethyst::lib

#endif  /* AMETHYST_LIB_RENDERMODEL_H */
