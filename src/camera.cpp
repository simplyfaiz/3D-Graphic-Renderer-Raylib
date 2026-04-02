#include "camera.h"
#include "vector3D.h"

void MyCamera::move(const Vec3 &delta) 
{
    position.x_ += delta.x_;
    position.y_ += delta.y_;
    position.z_ += delta.z_;
}

void MyCamera::rotate(const Vec3 &delta) 
{
    rotation.x_ += delta.x_;
    rotation.y_ += delta.y_;
    rotation.z_ += delta.z_;
}