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
        Scene_Object_Base(void);
        virtual ~Scene_Object_Base(void);

        virtual void render(const lib::Cartesian_Vector& reference) = 0;

};


class Scene_Object : public Scene_Object_Base, public lib::Object
{
    public:
        Model *model;

        void render(const lib::Cartesian_Vector& reference);
};


class Scene_Ship : public Scene_Object_Base, public lib::Ship
{
    public:
        Model *model;

        void render(const lib::Cartesian_Vector& reference);
};


class Scene_Star : public Scene_Object_Base //, public lib::Star  FIXME No stars in library yet.
{
    public:
        Model *model;

        void render(const lib::Cartesian_Vector& reference);
};


class Scene_Planet : public Scene_Object_Base //, public lib::Planet  FIXME No Planets in library yet.
{
    public:
        Model *model;

        void render(const lib::Cartesian_Vector& reference);
};

typedef boost::shared_ptr<Scene_Object> Scene_Object_ptr;
typedef boost::shared_ptr<Scene_Ship>   Scene_Ship_ptr;
typedef boost::shared_ptr<Scene_Star>   Scene_Star_ptr;
typedef boost::shared_ptr<Scene_Planet> Scene_Planet_ptr;

} // namespace client
} // namespace amethyst

#endif
