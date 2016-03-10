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

#include <memory>

namespace amethyst {
namespace client {


class Scene_Object_Base
{
    public:
        typedef std::shared_ptr<Scene_Object_Base> sptr;
        typedef std::weak_ptr<Scene_Object_Base>   wptr;

        Scene_Object_Base(void);
        virtual ~Scene_Object_Base(void);

        //Model::sptr model;

        //virtual void render(const lib::Cartesian_Vector& reference);
        virtual void render(const TransMatrix& m_proj, const TransMatrix& m_view, const TransMatrix& m_model);
};


class Scene_Object : public Scene_Object_Base, public lib::Object
{
    public:
        typedef std::shared_ptr<Scene_Object> sptr;
        typedef std::weak_ptr<Scene_Object>   wptr;
        
        //virtual void render(const lib::Cartesian_Vector& reference);
        virtual void render(const TransMatrix& m_proj, const TransMatrix& m_view, const TransMatrix& m_model);
};


class Scene_Ship : public Scene_Object_Base, public lib::Ship
{
    public:
        typedef std::shared_ptr<Scene_Ship> sptr;
        typedef std::weak_ptr<Scene_Ship>   wptr;

        //virtual void render(const lib::Cartesian_Vector& reference);
        //virtual void render(const TransMatrix& m_proj, const TransMatrix& m_view, const TransMatrix& m_model);
};


class Scene_Star : public Scene_Object_Base //, public lib::Star  FIXME No stars in library yet.
{
    public:
        typedef std::shared_ptr<Scene_Star> sptr;
        typedef std::weak_ptr<Scene_Star>   wptr;
};


class Scene_Planet : public Scene_Object_Base //, public lib::Planet  FIXME No Planets in library yet.
{
    public:
        typedef std::shared_ptr<Scene_Planet> sptr;
        typedef std::shared_ptr<Scene_Planet> wptr;
};


} // namespace client
} // namespace amethyst

#endif
