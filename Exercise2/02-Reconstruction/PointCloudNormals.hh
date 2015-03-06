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
//=============================================================================
//
//  CLASS PointCloudNormals
//
//=============================================================================

#ifndef POINT_CLOUD_NORMALS_HH_
#define POINT_CLOUD_NORMALS_HH_

#include "../eigen/eigen_include.h"
#include "../KdTree/KdTree.h"
#include <vector>
#include <utility>

class PointCloudNormals
{
public:
    PointCloudNormals(const std::vector<surface_mesh::Point>& _points, int _kdtree_query_size);

    ~PointCloudNormals()
    {
        if(kd_tree_) delete kd_tree_;
    }

    const std::vector<surface_mesh::Vec3>& get_normals() const
    {
        return out_normals_;
    }

    static float average_point_distance(const std::vector<surface_mesh::Point>& _points);

private:

    // Compute normal direcitons at each point using PCA of its neighboring points
    void compute_normals();

    // Consistently orient the normals using MST
    void MST_orient_normals();

    // initiliaze the point and normal matrices
    void init(const std::vector<surface_mesh::Point>& _points);

    // Compute the eigenvector of the smallest eigenvalue, for a 3X3 symmetric matrix
    Vector3d smallest_eigenvector(const Matrix3 &mat);

    // Edge weigths for MST propagation of normals
    double normal_weight(int i, int j);

    // Orient the normal at target_idx according to the one in source_idx
    void orient_normal(int source_idx, int target_idx);

    // Compute a minimum spanning tree of the points based on their Euclidean distance
    void compute_euclidean_MST(int n_knn_query_neighbors, std::vector<std::pair<int, int> > &edges);

protected:

    // Kd-tree for the input points
    ClosestPoints<double> *kd_tree_;

    // input points and their computed normals, stored in double-precision Eigen format
    Matrix3X points_, normals_;

    // the computed normals in surface_mesh vector format
    std::vector<surface_mesh::Vec3> out_normals_;

    int n_neighbors_;
};

#endif
