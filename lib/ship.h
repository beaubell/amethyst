#ifndef AMETHYST_LIB_SHIP_H
#define AMETHYST_LIB_SHIP_H

/***********************************************************************
 Amethyst-lib
  - Ship Object Class declarations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "object.h"

#include <boost/shared_ptr.hpp>

namespace amethyst {
namespace lib {


class Ship : public Object
{
   public:
    Ship();
    ~Ship();

    virtual void iterate(const double &time);

    void set_throttle(const float &position);
    void set_thrust_x(const float &position);
    void set_thrust_z(const float &position);

    void set_att_thrust_roll(const float &position);
    void set_att_thrust_pitch(const float &position);
    void set_att_thrust_yaw(const float &position);


   private:
    bool thruster_changed_;

    float throttle_pos_;
    float thrust_roll_pos_;
    float thrust_pitch_pos_;
    float thrust_yaw_pos_;

    Cartesian_Vector thrust_;
};

typedef boost::shared_ptr<Ship> Ship_ptr;

} // namespace lib
} // namespace amethyst

#endif
