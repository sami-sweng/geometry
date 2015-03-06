//== INCLUDES =================================================================


#include "Grid.h"


//== IMPLEMENTATION ==========================================================


Grid::
Grid(const Vec3f&  _origin,
     const Vec3f&  _x_axis,
     const Vec3f&  _y_axis,
     const Vec3f&  _z_axis,
     unsigned int  _x_res,
     unsigned int  _y_res,
     unsigned int  _z_res)
{
    // store bounding box
    origin_ = _origin;
    x_axis_ = _x_axis;
    y_axis_ = _y_axis;
    z_axis_ = _z_axis;

    // store grid resolution
    x_res_ = _x_res;
    y_res_ = _y_res;
    z_res_ = _z_res;

    // allocate scalar values
    values_.clear();
    values_.resize(x_res_*y_res_*z_res_, 0.0);

    // spacing
    dx_ = x_axis_ / (float)(x_res_-1);
    dy_ = y_axis_ / (float)(y_res_-1);
    dz_ = z_axis_ / (float)(z_res_-1);
}


//=============================================================================
