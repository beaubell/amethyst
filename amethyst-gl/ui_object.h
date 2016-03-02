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
    
    virtual void setPosition(const glm::vec2& newloc);
    virtual const glm::vec2& getPosition();
    
    virtual void setVisible(bool vis);
    virtual bool isVisible();
    
    virtual void render(const TransMatrix& proj, const TransMatrix& mat) = 0;
    virtual void update() = 0;
    
//private:
    glm::vec2 _position;
    //glm::vec2 _size;
    
    bool _visibile;
};

} // namespace client
} // namespace amethyst

#endif // UI_OBJECT_H
