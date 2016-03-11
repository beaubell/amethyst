#ifndef AMETHYST_UI_TEXTBOX_H
#define AMETHYST_UI_TEXTBOX_H

#include "ui_widget.h"
#include "opengl_shader.h"
#include "lib/types.h"
#include "FTGLTextHandle.h"
#include "FTFont.h"

namespace amethyst {
namespace client {

class UI_TextBox : public UI_Widget
{
  public:
    typedef std::shared_ptr<UI_TextBox> sptr;
    UI_TextBox(ftgl::FTFont &font, ShaderProgram::sptr shader);
    virtual ~UI_TextBox();
   
    virtual void render(const TransMatrix& proj, const TransMatrix& mat);
    virtual void update();

    void setText(const std::string& text);

    std::string _text;

  private:
    ShaderProgram::sptr _shaderptr;
    ftgl::FTFont &_font;
    ftgl::TextHandle _titlehdl;

    // Vertex Attribute Locations
    ShaderProgram::AttribHDL fnvertexLoc;
    ShaderProgram::AttribHDL fntexcoordLoc;
 
    // Uniform variable Locations
    ShaderProgram::UniformHDL fncolorLoc;
    ShaderProgram::UniformHDL fnprojMatrixLoc, fnviewMatrixLoc;
    ShaderProgram::UniformHDL fntexUnitLoc;
    
};

} // namespace client
} // namespace amethyst

#endif // UI_WIDGET_H
