//=============================================================================
//
//   A header-only wrapper for nanoflann, for closest points query using K-d trees.
//
//   Copyright (C) 2015 Bailin Deng <bldeng@gmail.com>
//   Copyright (C) 2015 Computer Graphics and Geometry Laboratory, EPF Lausanne
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

#ifndef KD_TREE_H_
#define KD_TREE_H_

#include "../eigen/eigen_include.h"
#include "nanoflann.hpp"
#include <vector>

template<typename ScalarT>
class ClosestPoints{

protected:

    // nanoflann point cloud adaptor based on Eigen matrices
    // Unlike the default Eigen matrix adaptor from nanoflann, it asssumes the points are stored as matrix columns,
    // which results in better performance for column-major matrices.

    struct PointCloudMatrixAdaptor
    {
        typedef Eigen::Matrix<ScalarT, 3, Eigen::Dynamic> PointCloudMatrix;

        const PointCloudMatrix &points; //!< A const ref to the data set origin

        /// The constructor that sets the data set source
        PointCloudMatrixAdaptor(const PointCloudMatrix &points_) : points(points_) { }

        /// Must return the number of data points
        inline size_t kdtree_get_point_count() const { return points.cols(); }

        /// Returns the squared distance between the vector "p1[0:size-1]" and the data point with index "idx_p2" stored in the class:
        inline ScalarT kdtree_distance(const ScalarT *p1, const size_t idx_p2,size_t /*size*/) const
        {
            return (points.col(idx_p2) - Eigen::Map< const Eigen::Matrix<ScalarT, 3, 1> >(p1)).squaredNorm();
        }

        // Returns the dim'th component of the idx'th point in the class:
        // Since this is inlined and the "dim" argument is typically an immediate value, the
        //  "if/else's" are actually solved at compile time.
        inline ScalarT kdtree_get_pt(const size_t idx, int dim) const
        {
            return points(dim, idx);
        }

        // Optional bounding-box computation: return false to default to a standard bbox computation loop.
        //   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
        //   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
        template <class BBOX>
        bool kdtree_get_bbox(BBOX& /*bb*/) const { return false; }

    }; // end of PointCloudAdaptor

    PointCloudMatrixAdaptor point_cloud;

    // K-d tree adaptor type from flann
    typedef nanoflann::KDTreeSingleIndexAdaptor<
        nanoflann::L2_Simple_Adaptor< ScalarT, PointCloudMatrixAdaptor > ,
        PointCloudMatrixAdaptor,
        3
        > Kd_Tree_Adaptor;

    Kd_Tree_Adaptor kd_tree;

public:

    ClosestPoints(const typename PointCloudMatrixAdaptor::PointCloudMatrix &points_)
        :point_cloud(points_), kd_tree(3, point_cloud, nanoflann::KDTreeSingleIndexAdaptorParams())
    {
        kd_tree.buildIndex();
    }

    /// Search for n_results nearst points among the input point array, and return their indices
    std::vector<size_t> knn_search(const Eigen::Matrix<ScalarT, 3, 1> &query_point, int n_results) const{
        std::vector<size_t> indices(n_results);
        std::vector<ScalarT> query_dist_sq(n_results);

        kd_tree.knnSearch(query_point.data(), n_results, &indices[0], &query_dist_sq[0]);
        return indices;
    }

};

#endif

