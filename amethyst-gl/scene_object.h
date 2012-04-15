#ifndef AMETHYST_CLIENT_SCENE_OBJECT_H
#define AMETHYST_CLIENT_SCENE_OBJECT_H

/***********************************************************************
 Amethyst-GL
  - ??? FIXME ??? declarations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "lib/object.h"
#include "lib/ship.h"
#include "model.h"

#include <boost/shared_ptr.hpp>

namespace amethyst {
namespace client {


class Scene_Object_Base
{
    public:
        typedef boost::shared_ptr<Scene_Object_Base> ptr;
        Scene_Object_Base(void);
        virtual ~Scene_Object_Base(void);

        virtual void render(const lib::Cartesian_Vector& reference) = 0;

};


class Scene_Object : public Scene_Object_Base, public lib::Object
{
    public:
        typedef boost::shared_ptr<Scene_Object> ptr;
        Model *model;

        void render(const lib::Cartesian_Vector& reference);
};


class Scene_Ship : public Scene_Object_Base, public lib::Ship
{
    public:
        typedef boost::shared_ptr<Scene_Ship> ptr;
        Model *model;

        void render(const lib::Cartesian_Vector& reference);
};


class Scene_Star : public Scene_Object_Base //, public lib::Star  FIXME No stars in library yet.
{
    public:
        typedef boost::shared_ptr<Scene_Star> ptr;
        Model *model;

        void render(const lib::Cartesian_Vector& reference);
};


class Scene_Planet : public Scene_Object_Base //, public lib::Planet  FIXME No Planets in library yet.
{
    public:
        typedef boost::shared_ptr<Scene_Planet> ptr;
        Model *model;

        void render(const lib::Cartesian_Vector& reference);
};


} // namespace client
} // namespace amethyst

#endif
