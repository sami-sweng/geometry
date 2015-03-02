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
//  CLASS MeshViewer - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================


#include <surface_mesh/IO.h>
#include "MeshViewer.hh"
#include "gl.hh"
#include <iostream>
#include <fstream>


//== IMPLEMENTATION ========================================================== 


MeshViewer::
MeshViewer(const char* _title, int _width, int _height)
  : GlutExaminer(_title, _width, _height)
{
  clear_draw_modes();
  add_draw_mode("Wireframe");
  add_draw_mode("Hidden Line");
  add_draw_mode("Solid Flat");
  add_draw_mode("Solid Smooth");
  set_draw_mode(3);
}

//-----------------------------------------------------------------------------

MeshViewer::~MeshViewer(){}


//-----------------------------------------------------------------------------


bool
MeshViewer::
open_mesh(const char* _filename)
{
  // load mesh
  if (surface_mesh::read_mesh(mesh_, _filename))
  {
    // set center and radius
    Mesh::Vertex_iterator  v_it(mesh_.vertices_begin()),
                           v_end(mesh_.vertices_end());
    surface_mesh::Point            bbMin, bbMax;

    bbMin = bbMax = mesh_.position(*v_it);
    for (; v_it!=v_end; ++v_it)
    {
      bbMin.minimize(mesh_.position(*v_it));
      bbMax.maximize(mesh_.position(*v_it));
    }
    set_scene((bbMin + bbMax)*0.5, 0.5* norm(bbMin - bbMax));


    // compute face & vertex normals
    mesh_.update_face_normals();
    mesh_.update_vertex_normals();


    // update face indices for faster rendering
    update_face_indices();

    // initialize custom color property
    vcolor_ = mesh_.vertex_property<Color>("v:color", Color(1.0f));

    // info
    std::cerr << mesh_.n_vertices() << " vertices, "
	      << mesh_.n_faces()    << " faces\n";

    return true;
  }

  return false;
}


//-----------------------------------------------------------------------------


void
MeshViewer::
update_face_indices()
{
  Mesh::Face_iterator        f_it(mesh_.faces_begin()),
                             f_end(mesh_.faces_end());
  Mesh::Vertex_around_face_circulator  fv_c;

  indices_.clear();
  indices_.reserve(mesh_.n_faces()*3);

  for (; f_it!=f_end; ++f_it)
  {
      fv_c=mesh_.vertices(*f_it);
      indices_.push_back((*fv_c).idx());

      ++ fv_c;
      indices_.push_back((*fv_c).idx());

      ++ fv_c;
      indices_.push_back((*fv_c).idx());
  }
}


//-----------------------------------------------------------------------------


void 
MeshViewer::
draw(const std::string& _draw_mode)
{
  if (indices_.empty())
  {
    GlutExaminer::draw(_draw_mode);
    return;
  }



  if (_draw_mode == "Wireframe")
  {
    glDisable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnableClientState(GL_VERTEX_ARRAY);
    GL::glVertexPointer(vertex_positions_pointer());

    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, &indices_[0]);

    glDisableClientState(GL_VERTEX_ARRAY);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }


  else if (_draw_mode == "Hidden Line")
  {

	  glDisable(GL_LIGHTING);
	  glShadeModel(GL_SMOOTH);
	  glColor3f(0.0, 0.0, 0.0);

	  glEnableClientState(GL_VERTEX_ARRAY);
      GL::glVertexPointer(vertex_positions_pointer());

	  glDepthRange(0.01, 1.0);
	  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, &indices_[0]);
	  glDisableClientState(GL_VERTEX_ARRAY);
	  glColor3f(1.0, 1.0, 1.0);

	  glEnableClientState(GL_VERTEX_ARRAY);
      GL::glVertexPointer(vertex_positions_pointer());

	  glDrawBuffer(GL_BACK);
	  glDepthRange(0.0, 1.0);
	  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	  glDepthFunc(GL_LEQUAL);
	  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, &indices_[0]);

	  glDisableClientState(GL_VERTEX_ARRAY);
	  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	  glDepthFunc(GL_LESS);

  }


  else if (_draw_mode == "Solid Flat")
  {
    Mesh::Face_iterator        f_it(mesh_.faces_begin()),
                               f_end(mesh_.faces_end());
    Mesh::Vertex_around_face_circulator  fv_c;

    glEnable(GL_LIGHTING);
    glShadeModel(GL_FLAT);

    glBegin(GL_TRIANGLES);
    for (; f_it!=f_end; ++f_it)
    {
      GL::glNormal(face_normals()[*f_it]);

      fv_c = mesh_.vertices(*f_it);
      GL::glVertex(mesh_.position(*fv_c));

      ++fv_c;
      GL::glVertex(mesh_.position(*fv_c));

      ++fv_c;
      GL::glVertex(mesh_.position(*fv_c));
    }
    glEnd();
  }


  else if (_draw_mode == "Solid Smooth")
  {
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    GL::glVertexPointer(vertex_positions_pointer());
    GL::glNormalPointer(vertex_normals_pointer());

    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, &indices_[0]);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
  }
}


//=============================================================================


void
MeshViewer::
set_color(Mesh::Vertex v, const Color& col)
{
    vcolor_[v] = col;
}


//=============================================================================


Surface_mesh::Vertex_property<Normal>
MeshViewer::
vertex_normals()
{
    return mesh_.vertex_property<Normal>("v:normal");
}


//=============================================================================


Surface_mesh::Face_property<Normal>
MeshViewer::
face_normals()
{
    return mesh_.face_property<Normal>("f:normal");
}


//=============================================================================


Point*
MeshViewer::
vertex_positions_pointer()
{
    return &(mesh_.points()[0]);
}


//=============================================================================


Normal*
MeshViewer::
vertex_normals_pointer()
{
    return &(vertex_normals().vector()[0]);
}


//=============================================================================


Color*
MeshViewer::
vertex_colors_pointer()
{
    return &(vcolor_.vector()[0]);
}
