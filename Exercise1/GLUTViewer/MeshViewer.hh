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
//  CLASS MeshViewer
//
//=============================================================================


#ifndef MESH_VIEWER_WIDGET_HH
#define MESH_VIEWER_WIDGET_HH


//== INCLUDES =================================================================


#include "GlutExaminer.hh"
#include <surface_mesh/Surface_mesh.h>

//== CLASS DEFINITION =========================================================


class MeshViewer : public GlutExaminer
{
public:
   
  /// default constructor
  MeshViewer(const char* _title, int _width, int _height);

  virtual ~MeshViewer();

  /// open mesh
  virtual bool open_mesh(const char* _filename);

  /// update buffer with face indices
  void update_face_indices();

  /// draw the scene
  virtual void draw(const std::string& _draw_mode);


protected:

  typedef surface_mesh::Surface_mesh  Mesh;

  /// Helper function to set the color property for a given vertex
  void set_color(Mesh::Vertex v, const Color& col);


  /// Get the vertex normals property
  Mesh::Vertex_property<Normal> vertex_normals();

  /// Get the face normals property
  Mesh::Face_property<Normal> face_normals();

  /// Pointer to the vertex position array, used for rendering
  Point* vertex_positions_pointer();

  /// Pointer to the vertex position array, used for rendering
  Normal* vertex_normals_pointer();

  /// Pointer to the vertex color array, used for rendering
  Color* vertex_colors_pointer();

protected:

  Mesh                       mesh_;
  std::vector<unsigned int>  indices_;
  Mesh::Vertex_property<Color> vcolor_;
};


//=============================================================================
#endif // MESH_VIEWER_WIDGET_HH defined
//=============================================================================

