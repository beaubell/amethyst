#include "ui_object.h"

namespace amethyst {
namespace client {


UI_Object::UI_Object()
{

}

UI_Object::UI_Object(const UI_Object& other)
{

}

UI_Object::~UI_Object()
{

}

UI_Object& UI_Object::operator=(const UI_Object& other)
{

}

bool UI_Object::operator==(const UI_Object& other) const
{

}

void UI_Object::setPosition(glm::vec2 newpos)
{
    _position = newpos;
}


void UI_Object::Render(const TransMatrix& proj, const TransMatrix& mat)
{
  
}

void UI_Object::Update()
{
  
}


} // namespace client
} // namespace amethyst
