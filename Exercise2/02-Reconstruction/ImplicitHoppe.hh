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
//  CLASS ImplicitHoppe
//
//=============================================================================

//=============================================================================


#ifndef HOPPE_HH
#define HOPPE_HH


//=============================================================================


#include "PointCloudNormals.hh"

//=============================================================================


class ImplicitHoppe : public PointCloudNormals
{
public:
    ImplicitHoppe(const std::vector<surface_mesh::Point>& _points, int _kdtree_query_size)
        : PointCloudNormals(_points, _kdtree_query_size)
	{}


    // Compute signed distance function at point _p
    float operator()(const surface_mesh::Point& _p) const
    {
        Vector3d p = to_vector3d(_p);

        // Find closest point index
        std::vector<size_t> indices = kd_tree_->knn_search(p, 1);
        int idx = indices.front();

        // Point-to-plane distance
        Vector3d closest_pt = this->points_.col(idx);
        Vector3d N = this->normals_.col(idx);
        return N.dot(p - closest_pt);
    }
};


//=============================================================================
#endif // HOPPE_HH defined
//=============================================================================

