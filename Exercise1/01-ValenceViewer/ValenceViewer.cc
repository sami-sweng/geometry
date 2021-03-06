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
#include <limits>
#include <stdio.h>
#include <cmath>

using namespace std;


//== IMPLEMENTATION ========================================================== 

//global variable that stores the maximal and min values of vertex valence
unsigned int max_valence;
unsigned int min_valence;

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
    min_valence = std::numeric_limits<unsigned int>::max();

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
        //nn_incident_vertices and conversely for min
        if (nb_incident_vertices > max_valence)
            max_valence = nb_incident_vertices;
        if (nb_incident_vertices < min_valence)
            min_valence = nb_incident_vertices;

        //storing the number of incident vertices for the current vertex v 
        vvalence_[v]=nb_incident_vertices;
    }

    /////////////////////////////////////////////////////////////////////////////
}


//-----------------------------------------------------------------------------

//conversion function from HSL color model to RGB
//JavaScript implementation found on http://stackoverflow.com/questions/2353211/hsl-to-rgb-color-conversion
//ported by us to C++
float
hue2rgb(float p, float q, float t)
{
    if(t < 0.0f) t += 1.0f;
    if(t > 1.0f) t -= 1.0f;
    if(t < 1.0f/6.0f) return p + (q - p) * 6.0f * t;
    if(t < 1.0f/2.0f) return q;
    if(t < 2.0f/3.0f) return p + (q - p) * (2.0f/3.0f - t) * 6.0f;
    return p;
}

Color
hslToRGB(float h, float s, float l)
{
    float r, g, b;

    if(s == 0.0f){
        r = g = b = l;
    }
    else{
        float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
        float p = 2.0f * l - q;
        r = hue2rgb(p, q, h + 1.0f/3.0f);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1.0f/3.0f);
    }

    return Color(r,g,b);
}

void 
ValenceViewer::
color_coding()
{
    // EXERCISE 1.3 /////////////////////////////////////////////////////////////
    // Implement a color visualization of your choice that shows the valence of
    // each vertex of "mesh_". Store the color of each vertex using the method
    // set_color(Vertex, Color)

    //We will store a color for each used valence
    Color *listOfColors = new Color[max_valence];

    float hue, saturation, lightness, pc;

    //we work with the HUE color system
    //we map the min and max valence to hue from 0 to 0.7 (red to blue)
    saturation=1.0f;
    lightness=0.5f;

    for(unsigned int i = min_valence; i < max_valence; i++)
    {

        pc = ((float)i - (float)min_valence)/((float)max_valence - (float)min_valence);
        //we compensate for one vertice which valence is 22
        //(in the bunny model most of them are in the range 3-11)
        pc = pc*2.5f;
        hue = 0.7f * pc;
        listOfColors[i]=hslToRGB(hue,saturation,lightness);
    }

    //float red, green, blue;

    //for(int i=0; i<max_valence; i+=1)
    //{
    //    red = (1.0 * i) / 100;
    //    green = (1.0 * (100 - i)) / 100;
    //    blue = (1.0 * (1000 - i)) / 100;
    //    listOfColors[i] = Color(red, green, blue);
   // }

    Mesh::Vertex_iterator v_it, v_begin, v_end;

    v_begin = mesh_.vertices_begin();
    v_end = mesh_.vertices_end();

    //iterate through all the vertices of the mesh
    for( v_it = v_begin ; v_it != v_end; ++ v_it ){
        Mesh::Vertex v = *v_it;

        unsigned int nb_incident_vertices=vvalence_[v];

        set_color(v,listOfColors[nb_incident_vertices]);

    }

    delete listOfColors;


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
