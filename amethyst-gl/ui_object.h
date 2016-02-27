#ifndef AMETHYST_UI_OBJECT_H
#define AMETHYST_UI_OBJECT_H

#include <memory>
#include <glm/glm.hpp>

namespace amethyst {
namespace client {

class UI_Object
{
public:
    typedef std::shared_ptr<UI_Object> sptr;
    UI_Object();
    UI_Object(const UI_Object& other);
    virtual ~UI_Object() = 0;
    
    UI_Object& operator=(const UI_Object& other);
    bool operator==(const UI_Object& other) const;
    
    virtual void Render() = 0;
    virtual void Update() = 0;
    
    glm::vec2 location;
    glm::vec2 size;
};

} // namespace client
} // namespace amethyst

#endif // UI_OBJECT_H
