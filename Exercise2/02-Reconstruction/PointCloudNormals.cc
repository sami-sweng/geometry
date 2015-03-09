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
//  CLASS PointCloudNormals - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================

#include "PointCloudNormals.hh"
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/graph/connected_components.hpp>
#include <iostream>
#include <vector>
#include <iterator>
#include <deque>

using namespace boost;
typedef adjacency_list < vecS, vecS, undirectedS,
  no_property, property < edge_weight_t, double > > Graph;
typedef graph_traits < Graph >::edge_descriptor GraphEdge;
typedef graph_traits < Graph >::vertex_descriptor GraphVertex;

inline int num_connected_components(Graph &g)
{
    std::vector<int> component(num_vertices(g));
    return connected_components(g, &component[0]);
}

PointCloudNormals::PointCloudNormals(const std::vector<surface_mesh::Point>& _points, int _kdtree_query_size)
    :kd_tree_(NULL), n_neighbors_(_kdtree_query_size)
{
    init(_points);
    compute_normals();
    MST_orient_normals();
}

void PointCloudNormals::init(const std::vector<surface_mesh::Point>& _points)
{
    int n_points = _points.size();

    points_.resize(3, n_points);
    normals_.setZero(3, n_points);

    for(int i = 0; i < n_points; ++i)
    {
        points_.col(i) = to_vector3d(_points[i]);
    }

    kd_tree_ = new ClosestPoints<double>(points_);
}


Vector3d PointCloudNormals::smallest_eigenvector(const Matrix3 &mat)
{
    Eigen::SelfAdjointEigenSolver< Matrix3 > eig(mat);
    return eig.eigenvectors().col(0);
}


void PointCloudNormals::compute_normals()
{
    int n_points = points_.cols();

    for(int i = 0; i < n_points; ++ i)
    {
        // The 3x3 matrix used for determining the normal vector
        Eigen::Matrix3d M;

        // INSERT CODE:
        // 1) Use the method knn_search(...) of the data member kd_tree_, to obtain the indices of
        // n_neighbors_ + 1 points in the point cloud that are closest to the i-th point. Note that
        // the i-th point is stored in the i-th column of the matrix data member points_.

        // calclutating n_neighbors_ + 1 nearest points for i-th point
        std::vector<size_t> neighbour_indices = kd_tree_->knn_search(points_.col(i), n_neighbors_ + 1);

        // 2) Using these nearest neighbors, set up the matrix M for estimating the normal at the i-th point.
        // 2.1 calculate center point mid_point
        Eigen::Vector3d mid_point;
        mid_point.setZero();

        for(int j = 0; j <= n_neighbors_; ++j)
        {
            mid_point = mid_point + points_.col(neighbour_indices[j]);
        }

        mid_point = mid_point * (1.0/(n_neighbors_ + 1));


        // 2.2 setting up matrix D, used to compute matrix M
        Eigen::MatrixXd D(3,n_neighbors_+1);
        for(int j = 0; j <= n_neighbors_; ++j)
        {
            D.col(j) = points_.col(neighbour_indices[j]) - mid_point;
        }

        //2.3 compute matrix M from matrix D
        M = D * D.transpose();

        // 3) Call the member function smallest_eigenvector(...) to compute the normal vector
        // from the matrix M, and store it into the i-th column of the matirx data member normals_
        normals_.col(i) = smallest_eigenvector(M);

    }
}


// A class for indicating the existing of a undirect graph edge between two vertices
class EdgeFlag
{
public:
    EdgeFlag(int N):N_(N), flags_(N*N){}

    bool get(int i, int j)
    {
        return flags_[lower_index(i, j)];
    }

    void set(int i, int j, bool val)
    {
        flags_.set(lower_index(i, j), val);
    }

    void reset()
    {
        flags_.reset();
    }

private:
    int N_;
    boost::dynamic_bitset<> flags_;

    int lower_index(int i, int j)
    {
        return std::min(i, j) * N_ + std::max(i, j);
    }
};


double PointCloudNormals::normal_weight(int i, int j)
{
    return std::max(0.0, 1 - std::abs(normals_.col(i).dot(normals_.col(j))));
}


void PointCloudNormals::orient_normal(int source_idx, int target_idx)
{
    if(normals_.col(source_idx).dot(normals_.col(target_idx)) < 0.0){
        normals_.col(target_idx) *= -1.0;
    }
}


float PointCloudNormals::average_point_distance(const std::vector<surface_mesh::Point>& _points)
{
    int n_points = _points.size();

    if(n_points < 2){
        return 0;
    }

    Eigen::Matrix3Xf points(3, n_points);

    for(int i = 0; i < n_points; ++i)
    {
        points.col(i) = Eigen::Map<const Eigen::Vector3f>(_points[i].data());
    }

    ClosestPoints<surface_mesh::Scalar> kd_tree(points);

    float total_neighbor_distance = 0.0;
    int n_neighbors = 7;
    for(int i = 0; i < n_points; ++ i){
        std::vector<size_t> indices = kd_tree.knn_search(points.col(i), n_neighbors);

        float neighbor_dist = 0.0;
        int k = 0;
        for(std::vector<size_t>::iterator iter = indices.begin(); iter != indices.end(); ++ iter){
            int j = *iter;
            if(i != j){
                neighbor_dist += (points.col(i) - points.col(j)).norm();
                k ++;
            }
        }

        if(k != 0){
            total_neighbor_distance += neighbor_dist / k;
        }
    }

    return total_neighbor_distance / n_points;
}


void PointCloudNormals::compute_euclidean_MST(int n_knn_query_neighbors,
                                               std::vector<std::pair<int, int> > &edges)
{
    int N = points_.cols();
    Graph g(N);
    EdgeFlag edge_flag(N);

    int n_neighbors = n_knn_query_neighbors;

    // Add edges until the graph is one connected component
    while(true)
    {
        for(int i = 0; i < N; ++ i){
            std::vector<size_t> indices = kd_tree_->knn_search(points_.col(i), n_neighbors);

            for(std::vector<size_t>::iterator iter = indices.begin(); iter != indices.end(); ++ iter){
                int j = *iter;

                if(i != j && !edge_flag.get(i, j)){
                    edge_flag.set(i, j, true);
                    GraphEdge e = add_edge(i, j, g).first;
                    get(edge_weight, g)[e] = (points_.col(i) - points_.col(j)).norm();
                }
            }
        }

        // Check if the graph is connected
        if(num_connected_components(g) == 1){
            break;
        }

        n_neighbors = std::min(n_neighbors*2, N);
    }

    std::vector < GraphEdge > MST_g;
    kruskal_minimum_spanning_tree(g, std::back_inserter(MST_g));

    edges.clear();
    for(std::vector< GraphEdge >::iterator e_iter = MST_g.begin(); e_iter != MST_g.end(); ++ e_iter){
        int i = source(*e_iter, g), j = target(*e_iter, g);
        edges.push_back(std::make_pair(i, j));
    }
}

void PointCloudNormals::MST_orient_normals()
{
    // Construct a minimum spanning tree of all points based on the Euclidean distance
    int N = points_.cols();

    // Construct a graph with edge weights computed from their normals
    Graph g_normal(N);
    EdgeFlag edge_flag(N);

    for(int i = 0; i < N; ++ i){
        std::vector<size_t> indices = kd_tree_->knn_search(points_.col(i), n_neighbors_ + 1);

        for(std::vector<size_t>::iterator iter = indices.begin(); iter != indices.end(); ++ iter){
            int j = *iter;

            if(i != j && !edge_flag.get(i, j)){
                edge_flag.set(i, j, true);
                GraphEdge e = add_edge(i, j, g_normal).first;
                get(edge_weight, g_normal)[e] = normal_weight(i, j);
            }
        }
    }

    // If g_normal is not connected, add the edges from the Euclidean MST
    if(num_connected_components(g_normal) != 1){
        std::vector< std::pair<int, int> > euclidean_MST_edges;
        compute_euclidean_MST(n_neighbors_ * 2 + 2, euclidean_MST_edges);

        for(std::vector< std::pair<int, int> >::iterator iter = euclidean_MST_edges.begin();
            iter != euclidean_MST_edges.end(); ++ iter){
            int i = iter->first, j = iter->second;

            if(i != j && !edge_flag.get(i, j)){
                edge_flag.set(i, j, true);
                GraphEdge e = add_edge(i, j, g_normal).first;
                get(edge_weight, g_normal)[e] = normal_weight(i, j);
            }
        }
    }

    std::vector < GraphEdge > MST_normal;
    kruskal_minimum_spanning_tree(g_normal, std::back_inserter(MST_normal));

    // Propagate the normals along using MST_normal

    // Build up an adjacency structure for the MST
    std::vector< std::vector<int> > MST_adjacency(N);
    for(std::vector<GraphEdge>::iterator e_iter = MST_normal.begin(); e_iter != MST_normal.end(); ++ e_iter){
        int i = source(*e_iter, g_normal), j = target(*e_iter, g_normal);
        MST_adjacency[i].push_back(j);
        MST_adjacency[j].push_back(i);
    }

    // First determine the normal direciton of the normal with the largest Z coordinate, and oriten the normals towards Z direction
    int i_max_Z = 0;
    Eigen::Map<DenseVector, Eigen::Unaligned, Eigen::InnerStride<3> > z_coord(points_.data() + 2, N);
    z_coord.maxCoeff(&i_max_Z);
    if(normals_(2, i_max_Z) < 0){
        normals_.col(i_max_Z) *= -1.0;
    }

    // BFS to propagate the normal orientations
    std::vector<int> current_indices, next_indices;
    current_indices.push_back(i_max_Z);
    boost::dynamic_bitset<> visited(N);

    while(!current_indices.empty()){
        next_indices.clear();

        for(std::vector<int>::iterator iter1 = current_indices.begin(); iter1 != current_indices.end(); ++ iter1){
            visited.set(*iter1, true);
            std::vector<int> &adj_vtx = MST_adjacency[*iter1];

            for(std::vector<int>::iterator iter2 = adj_vtx.begin(); iter2 != adj_vtx.end(); ++ iter2){
                if(!visited[*iter2]){
                    orient_normal(*iter1, *iter2);
                    next_indices.push_back(*iter2);
                }
            }
        }

        current_indices = next_indices;
    }

    // Write to out_normals_
    out_normals_.clear();
    for(int i = 0; i < N; ++ i){
        out_normals_.push_back(from_vector3d(normals_.col(i)));
    }
}
