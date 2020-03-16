/***********************************************************************
 Amethyst-lib
  - Physics Object Class Implementation

 Authors (c):
 2006-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include <stdlib.h>
#include <string>

#include "physics.h"
#include "object.h"

#include "yaml-cpp/yaml.h"


namespace amethyst {
namespace lib {

    Object::Object()
    : mass(1)
    {
        //printf("NEW OBJECT!");
    }

    Object::Object(const Object &right)
    {
        mass = right.mass;

        location     = right.location;
        velocity     = right.velocity;
        acceleration = right.acceleration;
        force        = right.force;

        // Rotation
        attitude             = right.attitude;
        angular_velocity     = right.angular_velocity;
        angular_acceleration = right.angular_acceleration;

        name = right.name;

    }

#if 0
    Object::~Object()
    {

        //printf("DEAD OBJECT");
    }
#endif

    void Object::force_add(const Cartesian_Vector &new_force)
    {
        force += new_force;
    }


    void Object::force_add(const Spherical_Vector &sphr_force)
    {
        Cartesian_Vector new_force = phys_alias_transform (sphr_force);

        force += new_force;
    }


    void Object::force_clear()
    {
        force.clear();
        acceleration.clear();
    }


    void Object::force_apply(void)
    {
        //velocity = phys_alibi_transform (velocity, acceleration, time);
        acceleration = force / mass;
    }


    void Object::accel_apply(const double &time)
    {
        Cartesian_Vector temp(acceleration * time);
        velocity += temp;
    }


    void Object::velocity_apply(const double &time)
    {
        Cartesian_Vector temp(velocity * time);
        location += temp;
    }


    void Object::iterate(const double &time)
    {


        // Apply rotation tranformations;
        //Quaternion acc_temp;
        //acc_temp.slerp(Quaternion(), angular_acceleration, time);
        //angular_velocity *= acc_temp;

        //Quaternion vel_temp;
        Quaternion future_angvelocity = angular_velocity;
        future_angvelocity *= angular_acceleration;
        angular_velocity.slerp(angular_velocity, future_angvelocity, time);


        Quaternion future_attitude(attitude);
        future_attitude *= angular_velocity;
        Quaternion current_attitude = attitude;
        attitude.slerp(current_attitude, future_attitude, time);

        //attitude         *= angular_acceleration;

        attitude.normalize();

        // Apply location transformations
        force_apply();
        accel_apply(time);
        velocity_apply(time);
    }
    
YAML::Node
Object::toYAML(){

    using namespace YAML;
    
    Node object;
    
    object["name"] = name;

    if (model) {
        object["model"] = model->getName();
    }
    
    object["mass"] = mass;
    object["location"] = location.toYAML();
    object["velocity"] = velocity.toYAML();
    object["acceleration"] = acceleration.toYAML();

    object["attitude"] = attitude.toYAML();
    object["angular_velocity"] = angular_velocity.toYAML();
    object["angular_acceleration"] = angular_acceleration.toYAML();

    return object;
    
}


void
Object::fromYAML(const YAML::Node object){

    using namespace YAML;

    name = object["name"].as<std::string>();

    mass = object["mass"].as<float_type>();
    Node yl = object["location"];
    if (yl.IsDefined())
        location.fromYAML(yl);

    Node yv = object["velocity"];
    if (yv.IsDefined())
        velocity.fromYAML(yv);

    Node ya = object["acceleration"];
    if (ya.IsDefined())
        acceleration.fromYAML(ya);

    Node yatt = object["attitude"];
    if (yatt.IsDefined())
        attitude.fromYAML(yatt);

    Node yav  = object["angular_velocity"];
    if (yav.IsDefined())
        angular_velocity.fromYAML(yav);

    Node yaa  = object["angular_acceleration"];
    if (yaa.IsDefined())
        angular_acceleration.fromYAML(yaa);
}


std::ostream&
operator<<(std::ostream& os, const Object& obj) {

    os << "Object Summary: " << obj.name << '\n';
    os << " Mass         : " << obj.mass << '\n';
    os << " Location     : " << obj.location << '\n';
    os << " Acceleration : " << obj.acceleration << '\n';
    os << " Force        : " << obj.force << '\n';

    return os;
}


} // namespace lib
} // namespace amethyst
