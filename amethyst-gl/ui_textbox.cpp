#include "ui_textbox.h"
#include "glm/gtc/matrix_transform.hpp"

namespace amethyst {
namespace client {

UI_TextBox::UI_TextBox(ftgl::FTFont &font, ShaderProgramFont::sptr shader)
 :UI_Widget(),
  _shaderptr(shader),
  _font(font),
  _titlehdl()
{
}

UI_TextBox::~UI_TextBox()
{
}

void UI_TextBox::render(const TransMatrix& m_proj, const TransMatrix& m_view)
{
    _shaderptr->use();
    _shaderptr->setProjM(m_proj);
    _shaderptr->setViewM(m_view);
    _shaderptr->setColorV4(Color(1.0f, 1.0f, 1.0f, 1.0f));
    _shaderptr->setTexUnit(0);

    _titlehdl.render();
}

void UI_TextBox::update()
{
}

void UI_TextBox::setText(const std::string& newtext)
{
    _text = newtext;
    _shaderptr->use();
    _font.Compose(_text, _titlehdl);
    _titlehdl.bind(_shaderptr->getHandle(), _shaderptr->getVertexLoc(), _shaderptr->getTexCoordLoc(), -1, -1);
}


} // namespace client
} // namespace amethyst
