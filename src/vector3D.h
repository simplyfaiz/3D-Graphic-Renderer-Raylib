#pragma once 

class Vec3
{
  public:
    float x_;
    float y_;
    float z_;

    Vec3() : x_(0), y_(0), z_(0) {}
    Vec3(float x, float y, float z) : x_(x), y_(y), z_(z) {}
};