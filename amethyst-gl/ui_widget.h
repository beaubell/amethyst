#ifndef AMETHYST_UI_WIDGET_H
#define AMETHYST_UI_WIDGET_H

#include "ui_object.h"

namespace amethyst {
namespace client {

class UI_Widget : public UI_Object
{
  public:
    typedef std::shared_ptr<UI_Widget> sptr;
    UI_Widget();
   ~UI_Widget();
};

} // namespace client
} // namespace amethyst

#endif // UI_WIDGET_H
