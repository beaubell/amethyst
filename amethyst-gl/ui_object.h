#ifndef AMETHYST_UI_OBJECT_H
#define AMETHYST_UI_OBJECT_H

#include <memory>
#include <glm/glm.hpp>
#include "lib/types.h"

namespace amethyst {
namespace client {

class UI_Object
{
public:
    typedef std::shared_ptr<UI_Object> sptr;
    UI_Object();
    UI_Object(const UI_Object& other);
    virtual ~UI_Object();
    
    UI_Object& operator=(const UI_Object& other);
    bool operator==(const UI_Object& other) const;
    
    virtual void setPosition(glm::vec2 newloc);
    
    virtual void Render(const TransMatrix& proj, const TransMatrix& mat);
    virtual void Update();
    
    glm::vec2 _position;
    glm::vec2 size;
};

} // namespace client
} // namespace amethyst

#endif // UI_OBJECT_H
