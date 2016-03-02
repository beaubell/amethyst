#include "ui_textbox.h"
#include "glm/gtc/matrix_transform.hpp"

namespace amethyst {
namespace client {

UI_TextBox::UI_TextBox(ftgl::FTFont &font, ShaderProgram::sptr shader)
 :UI_Widget(),
  _shaderptr(shader),
  _font(font),
  _titlehdl()
{
    fnvertexLoc     = _shaderptr->GetAttribLocation("vertexData");
    fntexcoordLoc   = _shaderptr->GetAttribLocation("texcoordData");

    fncolorLoc      = _shaderptr->GetUniformLocation("color"); 
    fnprojMatrixLoc = _shaderptr->GetUniformLocation("projMatrix");
    fnviewMatrixLoc = _shaderptr->GetUniformLocation("viewMatrix");
    fntexUnitLoc    = _shaderptr->GetUniformLocation("baseTex");
}

UI_TextBox::~UI_TextBox()
{
}

void UI_TextBox::render(const TransMatrix& proj, const TransMatrix& mat)
{
    _shaderptr->use();
    _shaderptr->UniformMatrix4f(fnprojMatrixLoc, proj);
    _shaderptr->UniformMatrix4f(fnviewMatrixLoc, mat);
    _shaderptr->Uniform4f(fncolorLoc, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    _shaderptr->Uniform1i(fntexUnitLoc.value , 0);

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
    _titlehdl.bind(_shaderptr->getHandle(), fnvertexLoc.value, fntexcoordLoc.value, -1, -1);
}


} // namespace client
} // namespace amethyst
