/***********************************************************************
 Amethyst-GL
  - A window class to show stats on object being viewed.

 Authors (c):
 2016 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "uiw_view.h"
#include "../global.h"
#include "../opengl.h"
#include "lib/utility.h"

namespace amethyst {
namespace client {

UIW_View::UIW_View(Amethyst_GL &amgl,  UI &ui)
 : UI_Window(ui, std::string("Camera Stats")),
   _amgl(amgl),
   tbViewing(std::make_shared<UI_TextBox>(ui.get_font(), ui.uifont_shader)),
   tbViewingMdl(std::make_shared<UI_TextBox>(ui.get_font(), ui.uifont_shader)),
   tbViewingTex(std::make_shared<UI_TextBox>(ui.get_font(), ui.uifont_shader)),
   tbTargeting(std::make_shared<UI_TextBox>(ui.get_font(), ui.uifont_shader)),
   tbPiloting(std::make_shared<UI_TextBox>(ui.get_font(), ui.uifont_shader))
{
  setPosition(glm::vec2(-350.0, 10.0));
  resize(glm::vec2(340.0, 100.0));

  glm::vec2 pos(10.0f, 10.0f);

  tbViewing->setText("Viewing: ");
  tbViewing->setPosition(pos);
  addWidget(tbViewing);

  pos.y += 10.0f; pos.x += 10.0f;
  tbViewingMdl->setText("Model: ");
  tbViewingMdl->setPosition(pos);
  addWidget(tbViewingMdl);
  
  pos.y += 10.0f;
  tbViewingTex->setText("Primitives: ");
  tbViewingTex->setPosition(pos);
  addWidget(tbViewingTex);
  
  pos.y += 20.0f; pos.x -= 10.0f;
  tbTargeting->setText("Targeting: ");
  tbTargeting->setPosition(pos);
  addWidget(tbTargeting);
  
  pos.y += 10.0f;
  tbPiloting->setText("Piloting: ");
  tbPiloting->setPosition(pos);
  addWidget(tbPiloting);
  update();
}

void UIW_View::update()
{
    std::string nullstr("NULL");
  
    std::string obj_name = "Viewing: " + (Global.obj_view)->name;
    tbViewing->setText(obj_name);
    
    if ((Global.obj_view)->model)
    {
      std::string viewmdl = "Model: " + (Global.obj_view)->model->getName();
      tbViewingMdl->setText(viewmdl);
      
      Model::sptr viewedmdl = dynamic_pointer_cast<Model>((Global.obj_view)->model);
      std::string viewprim = "Primitives:  " + std::to_string(viewedmdl->getPrimitives().size());
      tbViewingTex->setText(viewprim);
    }
    else
      tbViewingMdl->setText(nullstr);
    
    std::string targeting = "Targeting: " + (Global.obj_target)->name;
    tbTargeting->setText(targeting);
    
    std::string piloting = "Piloting: " + (Global.ship)->name;
    tbPiloting->setText(piloting);
}

} // namespace client
} // namespace amethyst

