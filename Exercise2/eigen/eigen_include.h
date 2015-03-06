//=============================================================================
//
//   Code framework for the lecture
//
//   "Digital 3D Geometry Processing"
//
//   Mark Pauly, Bailin Deng, Alexandru Ichim, Anastasia Tkach
//
//   Copyright (C) 2015 by Bailin Deng <bldeng@gmail.com>
//   Copyright (C) 2015 by Computer Graphics and Geometry Laboratory, EPF Lausanne
//
//-----------------------------------------------------------------------------
//
//                                License
//
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the GNU General Public License
//   as published by the Free Software Foundation; either version 2
//   of the License, or (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin Street, Fifth Floor,
//   Boston, MA  02110-1301, USA.
//
//=============================================================================

#ifndef EIGEN_INCLUDE_H_
#define EIGEN_INCLUDE_H_

#include "../surface_mesh/Surface_mesh.h"
#include <Eigen/Dense>
#include <Eigen/Sparse>

// Double-precision matrix and vector types from Eigen
typedef Eigen::Matrix3Xd Matrix3X;
typedef Eigen::Matrix3d Matrix3;
typedef Eigen::Vector3d Vector3d;
typedef Eigen::MatrixXd DenseMatrix;
typedef Eigen::VectorXd DenseVector;

// Conversion between double-precision eigen-type vector and single-precision surface_mesh vector
inline Vector3d to_vector3d(const surface_mesh::Vec3 &vec)
{
    return Vector3d(vec[0], vec[1], vec[2]);
}

inline surface_mesh::Vec3 from_vector3d(const Vector3d &vec)
{
    return surface_mesh::Vec3(vec[0], vec[1], vec[2]);
}


#endif
