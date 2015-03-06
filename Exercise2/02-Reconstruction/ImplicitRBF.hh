//=============================================================================
//
//   Code framework for the lecture
//
//   "Digital 3D Geometry Processing"
//
//   Mark Pauly, Mario Botsch, Balint Miklos, Hao Li, Sofien Bouaziz,
//   Yuliy Schwartzburg, Duygu Ceylan, Mario Deuss, Bailin Deng,
//   Alexandru Ichim, Anastasia Tkach
//
//   Copyright (C) 2007-2015 by Computer Graphics and Geometry Laboratory,
//         EPF Lausanne, Applied Geometry Group and Computer Graphics
//         Laboratory, ETH Zurich
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
//=============================================================================
//
//  CLASS ImplicitRBF
//
//=============================================================================

//=============================================================================

#ifndef RBF_HH
#define RBF_HH


//=============================================================================

#include "../eigen/eigen_include.h"
#include <vector>


//=============================================================================


class ImplicitRBF
{
public:
    ImplicitRBF(const std::vector<surface_mesh::Vec3>& _points, const std::vector<surface_mesh::Vec3>& _normals);

    // evaluate RBF at position _p
    double operator()(const surface_mesh::Vec3& _p) const;

private:

    // fit RBF to given constraints
    void compute_implicit_RBF();

    // evaluate RBF value at point _x, with center at _center
    static double kernel(const Vector3d& _center, const Vector3d& _x)
    {
        double r = (_x-_center).norm();
        return r*r*r;
    }

	// solve linear system _A * _x = _b
    void solve_linear_system(const DenseMatrix& _A, const DenseVector& _b, DenseVector& _x);

private:

    Matrix3X  points_, normals_;
    Matrix3X  centers_;
    DenseVector  weights_;
};


//=============================================================================
#endif // RBF_HH defined
//=============================================================================

