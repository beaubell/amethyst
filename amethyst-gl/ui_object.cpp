#include "ui_object.h"

namespace amethyst {
namespace client {


UI_Object::UI_Object()
: _visibile(true)
{

}

UI_Object::~UI_Object()
{

}

void UI_Object::setPosition(const glm::vec2& newpos)
{
    _position = newpos;
}

const glm::vec2& UI_Object::getPosition()
{
    return _position;
}

void UI_Object::setVisible(bool vis)
{
    _visibile = vis;
}

bool UI_Object::isVisible()
{
    return _visibile;
}


void UI_Object::render(const TransMatrix& proj, const TransMatrix& mat)
{
  
}

void UI_Object::update()
{
  
}


} // namespace client
} // namespace amethyst
