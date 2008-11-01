#ifndef INPUT_H
#define INPUT_H

/***********************************************************************
 Amethyst-GL
  - Input function declarations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/


namespace amethyst {
namespace client {


class Input
{
   public:
    Input();

    int process_inputs(void);

   private:
    /// Process Keyboard key down
    /// Process Keyboard key up
    /// Process

    /// flag for when alt key is held down.
    bool kb_alt;

    /// flag for when ctrl key is held down.
    bool kb_ctrl;

    /// flag for when shift key is held down.
    bool kb_shift;

    /// Flag for when mouse movement is controlling the camera.
    bool mouse_camera;

};


} // namespace client
} // namespace amethyst

#endif /* INPUT_H */
