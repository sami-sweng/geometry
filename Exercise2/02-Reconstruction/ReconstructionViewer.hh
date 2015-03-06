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
//  CLASS ReconstructionViewer
//
//=============================================================================

#ifndef RECONSTRUCTIONVIEWER_HH
#define RECONSTRUCTIONVIEWER_HH


//== INCLUDES =================================================================


#include <GLUTViewer/GlutExaminer.hh>
#include <surface_mesh/Surface_mesh.h>


//== CLASS DEFINITION =========================================================

	      

class ReconstructionViewer : public GlutExaminer
{
public:
   
    /// constructor
    ReconstructionViewer(const char* _title, int _width, int _height,
             const std::vector<surface_mesh::Vec3> *_points,
             double average_vertex_distance,
             const std::vector<surface_mesh::Vec3> *_normals = NULL,
             const surface_mesh::Surface_mesh *_hoppe_mesh = NULL,
             const surface_mesh::Surface_mesh *_rbf_mesh = NULL);

    // destructor
    virtual ~ReconstructionViewer();


protected:

    typedef surface_mesh::Surface_mesh  Mesh;

    virtual void draw(const std::string& _draw_mode);

    virtual void keyboard(int key, int x, int y);

    /// Get the vertex normals property
    Mesh::Vertex_property<Normal> vertex_normals();

    /// Get the face normals property
    Mesh::Face_property<Normal> face_normals();

    /// Pointer to the vertex position array, used for rendering
    Point* vertex_positions_pointer();

    /// Pointer to the vertex position array, used for rendering
    Normal* vertex_normals_pointer();

private:

    std::vector< Mesh >             mesh_;
    std::vector< std::vector<unsigned int> >  indices_;

    int current_mesh_idx_;

    std::vector< surface_mesh::Vec3 > points_, normal_end_points_;
    bool draw_points_, draw_normals_;
    float point_radius_;

    bool has_hoppe_mesh_, has_rbf_mesh_;

    Mesh& get_current_mesh(){
      return mesh_[current_mesh_idx_];
    }

    std::vector<unsigned int>& get_current_face_indices(){
        return indices_[current_mesh_idx_];
    }

    /// update buffer with face indices
    void update_face_indices(surface_mesh::Surface_mesh &mesh,
                             std::vector<unsigned int> &indices);

    void set_scene();
};


//=============================================================================
#endif // RECONSTRUCTIONVIEWER_HH defined
//=============================================================================

