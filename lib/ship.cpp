/***********************************************************************
 Amethyst-lib
  - Ship Object Class implementations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "ship.h"

namespace amethyst {
namespace lib {

Ship::Ship()
 : Object(),
   throttle_pos_(0.0f),
   thrust_roll_pos_(0.0f),
   thrust_pitch_pos_(0.0f),
   thrust_yaw_pos_(0.0f),
   thrust_()
{
}


Ship::~Ship()
{
}

void Ship::iterate(const double &time)
{
    /// Calculate attitude acceleration
    if (thruster_changed_)
    {
        thruster_changed_ = false;

        // FIXME - Make these meaningful scalers
        float scale_pitch = 0.5f;
        float scale_roll  = 0.5f;
        float scale_yaw   = 0.5f;

        Euler eul(thrust_pitch_pos_*scale_pitch, thrust_roll_pos_*scale_roll, thrust_yaw_pos_*scale_yaw);
        Quaternion quat(eul);

        angular_acceleration = quat;

    }

    /// Rotate thrust vectors to match orientation;
    float scale_throttle = 1e5;

    thrust_.clear();
    thrust_.y = throttle_pos_ * scale_throttle;

    force += QVRotate(attitude, thrust_);

    Object::iterate(time);
}

void Ship::set_throttle(const float &position)
{
    throttle_pos_ = position;
}

void Ship::set_att_thrust_roll(const float &position)
{
    thrust_roll_pos_  = position;
    thruster_changed_ = true;
}

void Ship::set_att_thrust_pitch(const float &position)
{
    thrust_pitch_pos_ = position;
    thruster_changed_ = true;
}

void Ship::set_att_thrust_yaw(const float &position)
{
    thrust_yaw_pos_   = position;
    thruster_changed_ = true;
}



} // namespace lib
} // namespace amethyst
