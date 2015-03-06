//=============================================================================


#ifndef GRID_H
#define GRID_H


//== INCLUDES =================================================================


#include <surface_mesh/Vector.h>
#include <vector>


//== CLASS DEFINITION =========================================================

using namespace surface_mesh;

class Grid
{
public:

    Grid(const Vec3f&  _origin = Vec3f(0,0,0),
         const Vec3f&  _x_axis = Vec3f(1,0,0),
         const Vec3f&  _y_axis = Vec3f(0,1,0),
         const Vec3f&  _z_axis = Vec3f(0,0,1),
         unsigned int  _x_res = 10,
         unsigned int  _y_res = 10,
         unsigned int  _z_res = 10);


    const Vec3f& origin() const { return origin_; }
    const Vec3f& x_axis() const { return x_axis_; }
    const Vec3f& y_axis() const { return y_axis_; }
    const Vec3f& z_axis() const { return z_axis_; }

    unsigned int x_resolution() const { return x_res_; }
    unsigned int y_resolution() const { return y_res_; }
    unsigned int z_resolution() const { return z_res_; }


    // 3D position of (x,y,z)
    Vec3f point(unsigned int x, unsigned int y, unsigned int z) const {
        return origin_ + dx_*x + dy_*y + dz_*z;
    }
    Vec3f point(const Vec3ui& xyz) const {
        return origin_ + dx_*xyz[0] + dy_*xyz[1] + dz_*xyz[2];
    }


    // scalar value at (x,y,z)
    float& operator()(unsigned int x, unsigned int y, unsigned int z) {
        return values_[z + y*z_res_ + x*z_res_*y_res_];
    }
    float operator()(unsigned int x, unsigned int y, unsigned int z) const {
        return values_[z + y*z_res_ + x*z_res_*y_res_];
    }
    float& operator()(const Vec3ui& xyz) {
        return values_[xyz[2] + xyz[1]*z_res_ + xyz[0]*z_res_*y_res_];
    }
    float operator()(const Vec3ui& xyz) const {
        return values_[xyz[2] + xyz[1]*z_res_ + xyz[0]*z_res_*y_res_];
    }


private:

    Vec3f               origin_, x_axis_, y_axis_, z_axis_, dx_, dy_, dz_;
    unsigned int        x_res_, y_res_, z_res_;
    std::vector<float>  values_;
};


//=============================================================================
#endif
//=============================================================================
