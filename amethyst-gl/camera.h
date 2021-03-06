#pragma once
/***********************************************************************
 Amethyst-GL
  - Camera class declarations

 Authors (c):
 2016-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "lib/orientation.h"
#include "glm/mat4x4.hpp"
#include "glm/vec2.hpp"

#include "yaml-cpp/node/node.h"

#include <memory>


namespace amethyst {
namespace client {

enum Eye {MONO = 0, LEFT = 1, RIGHT = 2, MAX = 3};

typedef glm::dmat4 PersMatrix;
typedef glm::dvec4 Position;
typedef glm::uvec2 Screen;
typedef glm::dvec2 CameraAngle;

struct PVMatrix
{
    PVMatrix() : proj(1), view(1) {};
    PersMatrix proj;
    PersMatrix view;
};


class Camera
{
  public:
    typedef std::shared_ptr<Camera> sptr;
    typedef std::weak_ptr<Camera>   wptr;

    Camera();
    virtual ~Camera();

    //Set Screen;
    void setScreen(const Screen& scrnsize);

    //Set Camera Attitude
    void setAttitude(const lib::Quaternion& attitude);

    //Generate Matrii
    void genMatProj() const;
    void genMatView() const;

    //Get Matrii
    const PVMatrix& getMatrii(const Eye eye = Eye::MONO) const;

    //Camera Position
    double getDistance() const;
    double getPitch() const;
    double getYaw() const;
    void setDistance(double newdistance);
    void setPitch(double newpitch);
    void setYaw(double newyaw);

    void incPitch(double addpitch);
    void incYaw(double addyaw);
    
    YAML::Node toYAML() const;
    void fromYAML(YAML::Node);
    

    //Get Camera Space Coords

    //Get Clip Space Coords

    //Get NDC Coords

  protected:
    mutable PVMatrix matrii_[3];
    Position cam_pos_;

  private:
    Camera(const Camera& rhs) = delete;
    Camera& operator=(const Camera&) = delete;
    glm::dvec2 screen;
    mutable double eyeseparation_;
    double yaw;
    double pitch;
    double distance;

    lib::Quaternion cam_att[10];
    int cam_num;
    lib::Quaternion cur_att;
};


} // namespace client
} // namespace amethyst
