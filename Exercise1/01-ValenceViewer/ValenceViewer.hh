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
//  CLASS ValenceViewer
//
//=============================================================================


#ifndef VALENCEVIEWERWIDGET_HH
#define VALENCEVIEWERWIDGET_HH


//== INCLUDES =================================================================


#include <GLUTViewer/MeshViewer.hh>



//== CLASS DEFINITION =========================================================

	      

class ValenceViewer : public MeshViewer
{
public:
   
  /// default constructor
  ValenceViewer(const char* _title, int _width, int _height);

  // destructor
  virtual ~ValenceViewer();

  /// open mesh
  virtual bool open_mesh(const char* _filename);



protected:

  virtual void draw(const std::string& _draw_mode);


  /// calculate vertex valences
  void calc_valences();

  /// set vertex color from vertex valence
  void color_coding();

private:

  Surface_mesh::Vertex_property<unsigned int>  vvalence_;
};


//=============================================================================
#endif // VALENCEVIEWERWIDGET_HH defined
//=============================================================================

