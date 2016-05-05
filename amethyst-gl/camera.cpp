/***********************************************************************
 Amethyst-GL
  - Camera class implementations

 Authors (c):
 2016-2016 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "camera.h"
#include <cmath>
#include "glm/gtc/matrix_transform.hpp"

namespace amethyst {
namespace client {

Camera::Camera()
: screen(1024.0,640.0),
  eyeseparation_(1.0),
  yaw(0),
  pitch(0),
  distance(2.0e8f),
  cam_num(0)
{

}

Camera::~Camera()
{

}

void Camera::setScreen(const Screen& newscreen)
{
    screen = newscreen;
}

void Camera::setAttitude(const lib::Quaternion& newattitude)
{
    // Ring buffer (delays camera movement by a few frames)
    {
        cam_num++;
        if(cam_num > 8) cam_num = 0;

        cam_att[cam_num] = newattitude;

        int view_num = cam_num - 7;
        if(view_num < 0) view_num = view_num + 9;
        cur_att  = cam_att[view_num];
    }
  
}

void Camera::genMatProj()
{
    // FIXME This struct needs to go away
    struct _cam {
      double near = 1.0;
      double aperture = 0.5;
      double fo = 100000.0;
    } camera;

    double widthdiv2 = camera.near * tan(camera.aperture/2);
    double aspectratio = screen.x / screen.y;

    double eyeoffset[Eye::MAX] = {0.0, 0.0, 0.0};

    eyeoffset[Eye::LEFT]  = -0.5 * eyeseparation_ * camera.near / camera.fo;
    eyeoffset[Eye::RIGHT] = +0.5 * eyeseparation_ * camera.near / camera.fo;

    double top = widthdiv2;
    double bottom = - widthdiv2;
    double near = 1;
    double far = 1e15f;

    for (int i = Eye::MONO; i < Eye::MAX; i++)
    {
        double left   = - aspectratio * widthdiv2 + eyeoffset[i] ;
        double right  =   aspectratio * widthdiv2 + eyeoffset[i] ;

        matrii_[i].proj = glm::frustum (left, right, bottom, top, near, far);
    }
}

void Camera::genMatView()
{
    // Convert to radians
    double x_rad = (yaw / 180.0) * M_PI;
    double y_rad = (pitch / 180.0) * M_PI;


    double eye_offset = 0.0;

    lib::Quaternion Qz( cos((x_rad)/2.0), 0.0, 0.0, sin((x_rad)/2.0));
    Qz.normalize();
    lib::Quaternion Qx( cos(y_rad/2.0), sin(y_rad/2.0), 0.0, 0.0 );
    Qx.normalize();

    lib::Quaternion new_att = cur_att * Qz * Qx;
    new_att.normalize();

    // Rotate Camera to ship's orientation
    lib::Cartesian_Vector raw_pos (0.0e11, 0.0, 0.0);
    lib::Cartesian_Vector raw_view(0.0, 1.0e11, 0.0);
    lib::Cartesian_Vector raw_up  (0.0, 0.0, 1.0e11);

    //Camera location in relation to ship
    lib::Cartesian_Vector shipoffset(0.0, -distance, 0.0);
    lib::Cartesian_Vector eyeoffset(eye_offset ,0.0, 0.0);

    lib::Cartesian_Vector real_pos   = (lib::QVRotate(new_att, (shipoffset + eyeoffset + raw_pos)));
    lib::Cartesian_Vector real_view  = (lib::QVRotate(new_att, (shipoffset + eyeoffset + raw_view)));
    lib::Cartesian_Vector real_up    = (lib::QVRotate(new_att, (shipoffset + eyeoffset + raw_up)));

    glm::dvec3 rw_pos  = glm::dvec3(real_pos.x, real_pos.y, real_pos.z);
    glm::dvec3 rw_view = glm::dvec3(real_view.x, real_view.y, real_view.z);
    glm::dvec3 rw_up   = glm::dvec3(real_up.x,   real_up.y,   real_up.z);

    // Generate Mono View Matrix
    matrii_[Eye::MONO].view = glm::lookAt(rw_pos, rw_view, rw_up);

    // Generate Stereo View Matrii
    auto v1 = rw_view - rw_pos;
    auto v2 = rw_up - rw_pos;

    auto rw_left_norm = glm::normalize(glm::cross(v2, v1));
    auto rw_left = rw_left_norm * eyeseparation_ * 0.5;

    matrii_[Eye::LEFT].view = glm::lookAt(rw_pos + rw_left, rw_view + rw_left, rw_up + rw_left);
    matrii_[Eye::RIGHT].view = glm::lookAt(rw_pos - rw_left, rw_view - rw_left, rw_up - rw_left);
}

PVMatrix& Camera::getMatrii(const Eye eye)
{
    //FIXME - Optimize the generation of the PV matrii
    genMatProj();
    genMatView();
    return matrii_[eye];
}

double Camera::getDistance()
{
    return distance;
}

double Camera::getPitch()
{
    return pitch;
}

double Camera::getYaw()
{
    return yaw;
}


void Camera::incPitch(double addpitch)
{
    pitch += addpitch;
    if (pitch < -90) pitch = -90;
    if (pitch >  90) pitch = 90;
}

void Camera::incYaw(double addyaw)
{
    yaw += addyaw;
    if (yaw < -180) yaw += 360;
    if (yaw >  180) yaw -= 360;
}

void Camera::setDistance(double newdistance)
{
    distance = newdistance;
}

void Camera::setPitch(double newpitch)
{
    pitch = newpitch;
}

void Camera::setYaw(double newyaw)
{
    yaw = newyaw;
}

} // namespace client
} // namespace amethyst
