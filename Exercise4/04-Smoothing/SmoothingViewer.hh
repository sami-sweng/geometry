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
//  CLASS SmoothingViewer
//
//=============================================================================


#ifndef SMOOTHING_VIEWER_HH
#define SMOOTHING_VIEWER_HH


//== INCLUDES =================================================================


#include "QualityViewer.hh"



//== CLASS DEFINITION =========================================================

	      

class SmoothingViewer : public QualityViewer
{
public:
   
  /// default constructor
  SmoothingViewer(const char* _title, int _width, int _height);

  /// iterative Laplacian smoothing
  void smooth(unsigned int _iters);

  void uniform_smooth(unsigned int _iters);

  void uniform_laplacian_enhance_feature();

  void laplace_beltrami_enhance_feature();

  bool initialized(){
      return init_success_;
  }

  /// open mesh
  virtual bool open_mesh(const char* _filename);


private:


  virtual void keyboard(int key, int x, int y);

  bool load_setting_file();



private:

  // Parameters for feature enhancement
  int enhancement_smoothing_iterations_;
  float enhancement_coef_;

  bool init_success_;
  Mesh::Vertex_property<Point>   v_new_pos_;
  Mesh::Vertex_property<Point>   v_old_pos_;
};


//=============================================================================
#endif // SMOOTHING_VIEWER_HH defined
//=============================================================================

