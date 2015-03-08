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
//  CLASS ImplicitRBF - IMPLEMENTATION
//
//=============================================================================

//=============================================================================


#include "ImplicitRBF.hh"


//== IMPLEMENTATION ==========================================================

ImplicitRBF::
ImplicitRBF(const std::vector<surface_mesh::Vec3>& _points, const std::vector<surface_mesh::Vec3>& _normals)
{
    int n = _points.size();
    points_.resize(3, n);
    normals_.resize(3, n);


    for(int i = 0; i < n; ++ i){
        points_.col(i) = to_vector3d(_points[i]);
        normals_.col(i) = to_vector3d(_normals[i]);
    }

    compute_implicit_RBF();
}

//-----------------------------------------------------------------------------

void
ImplicitRBF::
compute_implicit_RBF()
{
    std::cout << "Start Implicit RBF fitting" << std::endl;
    int n = points_.cols();
    int N = 2 * n;

    // Matrices for the RBF centers and their weights
    centers_.setZero(3, N);
    weights_.setZero(N);

    // Linear system matrix and right-hand-side vector for computing the weights
    DenseMatrix M(N, N);
    DenseVector d(N);


    // INSERT CODE:
    // 1) Determine the RBF centers, and store them into the columns of matrix centers_;
    //    The offset distance should be determined according to the bounding box diaginal length.

    // 1.1 calculating bounding box diagonal length
    Eigen::Vector3d max_bounding_corner;
    max_bounding_corner << points_.row(0).maxCoeff(), points_.row(1).maxCoeff(), points_.row(2).maxCoeff();
    Eigen::Vector3d min_bounding_corner;
    min_bounding_corner << points_.row(0).minCoeff(), points_.row(1).minCoeff(), points_.row(2).minCoeff();
    float L=(max_bounding_corner-min_bounding_corner).norm();

    // 1.2 setting epsilon value
    float r=0.001f;
    float epsilon=L*r;

    // 1.3 defining the 2n RBF centers
    for(int i = 0; i < n; ++ i)
    {
        centers_.col(i)=points_.col(i);
        centers_.col(i+n)=points_.col(i)+epsilon*normals_.col(i);
    }

    // 2) Collect the on- and off-surface constraints, to set up the linear system matrix M
    //    and the right-hand-side d.
    for(int i = 0; i < N; ++ i)
    {
        for (int j=0; j<N; ++j){
            M(i,j)=kernel(centers_.col(i),centers_.col(j));
        }
    }
    for (int i=0; i<n; ++i){
        d(i)=0.0f;
        d(i+n)=epsilon;
    }
    // 3) Use the memeber function solve_linear_system(...) to solve the linear system
    //    to obtain RBF weights, and store them in the data member weights_.
    solve_linear_system(M,d,weights_);
}


//-----------------------------------------------------------------------------


void
ImplicitRBF::
solve_linear_system(const DenseMatrix& _A, const DenseVector& _b, DenseVector& _x)
{
    // Solve the linear system using Eigen's Householder QR factorization
    Eigen::HouseholderQR<DenseMatrix> qr(_A);
    _x = qr.solve(_b);
}


//-----------------------------------------------------------------------------


double 
ImplicitRBF::
operator()(const surface_mesh::Vec3& _p) const
{
    Vector3d p = to_vector3d(_p);
    int N = centers_.cols();

    double f = 0.0;
    for(int i = 0; i < N; ++ i){
        f += weights_(i) * kernel(p, centers_.col(i));
    }

    return f;
}


//=============================================================================
