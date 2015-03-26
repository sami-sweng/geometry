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
//   Copyright (C) 2015 by Bailin Deng <bldeng@gmail.com>
//   Copyright (C) 2006-2015 by Computer Graphics and Geometry Laboratory, EPFL
//                              Computer Graphics Laboratory, ETH Zurich,
//                              and Computer Graphics Group, RWTH Aachen
//
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
//  CLASS QualityViewer
//
//=============================================================================


#ifndef QUALVIEWERWIDGET_HH
#define QUALVIEWERWIDGET_HH


//== INCLUDES =================================================================


#include "GLUTViewer/MeshViewer.hh"



//== CLASS DEFINITION =========================================================

	      

class QualityViewer : public MeshViewer
{
public:
   
  /// default constructor
  QualityViewer(const char* _title, int _width, int _height);

  // destructor
  ~QualityViewer();

  /// open mesh
  virtual bool open_mesh(const char* _filename);



protected:

  std::vector<float>  face_colors_;

  virtual void init();
  virtual void draw(const std::string& _draw_mode);


  /// calculate vertex and edge weights
  void calc_weights();

  void calc_edge_weights();
  void calc_vertex_weights();

  /// calculate mean curvature per vertex
  void calc_mean_curvature();
  void calc_uniform_mean_curvature();

  void calc_gauss_curvature();

  /// calculate triangle shape indices
  void calc_triangle_quality();
  void face_color_coding();

  Color value_to_color(Scalar value, Scalar min_value, Scalar max_value);

  /// set vertex color from vertex curvature
  void color_coding(Mesh::Vertex_property<Scalar> prop);

  /// initialize the proeprty variables from the mesh
  void init_properties();

  Mesh::Vertex_property<Scalar>  vweight_, vunicurvature_, vcurvature_, vgausscurvature_;
  Mesh::Edge_property<Scalar>  eweight_;
  Mesh::Face_property<Scalar>  tshape_;

  GLuint  textureID_;

};


//=============================================================================
#endif // QUALVIEWERWIDGET_HH defined
//=============================================================================

