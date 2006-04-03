/***************************************************************************
 *   Model loading and processing function implimentations                 *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include "model.h"
#include "global.h"

void DrawShip(void);

void load_models(void)
{
    // Build a single display list
    Global.dlShip = glGenLists(1);
    glNewList(Global.dlShip, GL_COMPILE);
    DrawShip();
    glEndList();
}

void free_models(void)
{
    glDeleteLists(Global.dlShip,1);

}
