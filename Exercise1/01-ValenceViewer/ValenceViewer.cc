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
//  CLASS ValenceViewer - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

#include "ValenceViewer.hh"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;


//== IMPLEMENTATION ========================================================== 

//global variable that stores the maximal value of vertex valence
unsigned int max_valence;

ValenceViewer::
ValenceViewer(const char* _title, int _width, int _height)
  : MeshViewer(_title, _width, _height)
{ 
  add_draw_mode("Vertex Valences");
}



//-----------------------------------------------------------------------------


ValenceViewer::
~ValenceViewer()
{
}

//-----------------------------------------------------------------------------

bool
ValenceViewer::
open_mesh(const char* _filename)
{
  // load mesh
  if (MeshViewer::open_mesh(_filename))
  {
    // compute vertex valence and color coding
    calc_valences();
    color_coding();

    glutPostRedisplay();
    return true;
  }
  return false;
}


//-----------------------------------------------------------------------------


void 
ValenceViewer::
calc_valences()
{
    // initialize the valence property of the mesh
    vvalence_ = mesh_.vertex_property<unsigned int>("v:valence", 0);

    // EXERCISE 1.2 /////////////////////////////////////////////////////////////
    // Compute valence of every vertex of "mesh_" and store them in vertex property vvalence_
    // (hint: use Mesh::Vertex_iterator)

    // Implement something here
    Mesh::Vertex_iterator v_it, v_begin, v_end;

    v_begin = mesh_.vertices_begin();
    v_end = mesh_.vertices_end();

    max_valence = 0;
    //iterate through all the vertices of the mesh
    for( v_it = v_begin ; v_it != v_end; ++ v_it ){
        Mesh::Vertex v = *v_it;
        Mesh::Vertex_around_vertex_circulator vc, vc_end;

        vc = mesh_.vertices(v);
        vc_end = vc;

        //iterate through all the incident vertices
        unsigned int nb_incident_vertices=0;
        do{
            nb_incident_vertices++;
        }
        while(++vc != vc_end);

        //if the current number of incident vertices is greater than
        //the current known maximal valence, we set the max_valence to be
        //nn_incident_vertices 
        if (nb_incident_vertices > max_valence)
            max_valence = nb_incident_vertices;

        //storing the number of incident vertices for the current vertex v 
        vvalence_[v]=nb_incident_vertices;
    }

    /////////////////////////////////////////////////////////////////////////////
}


//-----------------------------------------------------------------------------


void 
ValenceViewer::
color_coding()
{
    // EXERCISE 1.3 /////////////////////////////////////////////////////////////
    // Implement a color visualization of your choice that shows the valence of
    // each vertex of "mesh_". Store the color of each vertex using the method
    // set_color(Vertex, Color)

    // Implement something here
    //Color listOfColors[12]={
    //    Color(1.0f),
    //    Color(1.0f),
    //    Color(1.0f),
    //    Color(1.0f,0.0f,0.0f),
    //    Color(1.0f,0.5249999999999999f,0.0f),
    //    Color(0.95f,1.0f,0.0f),
    //    Color(0.4250000000000005f,1.0f,0.0f),
    //    Color(0.0f,1.0f,0.09999999999999998f),
    //    Color(0.0f,1.0f,0.6250000000000001f),
    //    Color(0.0f,0.8500000000000003f,1.0f),
    //    Color(0.0f,0.3250000000000002f,1.0f),
    //    Color(0.1999999999999993f,0.0f,1.0f)
    //};

    Color *listOfColors = new Color[max_valence];

    float hue, saturation, lightness;

    int color_index=0;
    //interval [0.0-0.7] for hue is splited in max_valence parts
    float part = 0.7 / max_valence;

    //we work with the HUE color system 
    for(float i = 0; i < 0.7; i += part)
    {
        hue = i;
        saturation = (90 + rand() * 10)/100;
        lightness = (50 + rand() * 10)/100;
        listOfColors[color_index]=Color(hue,saturation,lightness);
        color_index++;    
    }


    Mesh::Vertex_iterator v_it, v_begin, v_end;

    v_begin = mesh_.vertices_begin();
    v_end = mesh_.vertices_end();

    //iterate through all the vertices of the mesh
    for( v_it = v_begin ; v_it != v_end; ++ v_it ){
        Mesh::Vertex v = *v_it;

        unsigned int nb_incident_vertices=vvalence_[v];

        set_color(v,listOfColors[nb_incident_vertices]);

    }


    /////////////////////////////////////////////////////////////////////////////
}


//-----------------------------------------------------------------------------


void 
ValenceViewer::
draw(const std::string& _draw_mode)
{
    if (indices_.empty())
    {
        MeshViewer::draw(_draw_mode);
        return;
    }

    if (_draw_mode == "Vertex Valences")
    {
        glDisable(GL_LIGHTING);
        glShadeModel(GL_SMOOTH);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        GL::glVertexPointer(vertex_positions_pointer());
        GL::glNormalPointer(vertex_normals_pointer());
        GL::glColorPointer(vertex_colors_pointer());
        glDepthRange(0.01, 1.0);
        glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, &indices_[0]);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glColor3f(0.1, 0.1, 0.1);
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

    else MeshViewer::draw(_draw_mode);
}


//=============================================================================
