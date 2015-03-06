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
//  CLASS ReconstructionViewer - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

#include "ReconstructionViewer.hh"
#include <vector>
#include <algorithm>
#include <limits>
#include <ctype.h>



//== IMPLEMENTATION ========================================================== 


ReconstructionViewer::
ReconstructionViewer(const char* _title, int _width, int _height,
                     const std::vector<surface_mesh::Vec3> *_points,
                     double average_vertex_distance,
                     const std::vector<surface_mesh::Vec3> *_normals,
                     const surface_mesh::Surface_mesh *_hoppe_mesh,
                     const surface_mesh::Surface_mesh *_rbf_mesh)
  : GlutExaminer(_title, _width, _height), current_mesh_idx_(-1),
    draw_points_(false), draw_normals_(false), point_radius_(1.0),
    has_hoppe_mesh_(false), has_rbf_mesh_(false)
{
    clear_draw_modes();

    // Initialize point cloud data
    if(!_points->empty()){
        points_ = *_points;
        point_radius_ = average_vertex_distance * 0.2;
        draw_points_ = true;

        if(_normals){
            for(int i = 0; i < static_cast<int>(points_.size()); ++ i)
            {
                normal_end_points_.push_back(points_[i] + _normals->at(i) * average_vertex_distance);
            }

            draw_normals_ = true;
        }

        add_draw_mode("Points");
    }

    if(points_.empty() && !_hoppe_mesh && !_rbf_mesh){
        add_draw_mode("Solid Smooth");
        set_draw_mode(0);
    }
    else if(_hoppe_mesh || _rbf_mesh){
        add_draw_mode("Solid Smooth");
        add_draw_mode("Solid Flat");
        add_draw_mode("Wireframe");
        add_draw_mode("Hidden Line");
        set_draw_mode(_points->empty() ? 0 : 1);
    }

    if(_hoppe_mesh){
        mesh_.push_back(*_hoppe_mesh);
        has_hoppe_mesh_ = true;
    }

    if(_rbf_mesh){
        mesh_.push_back(*_rbf_mesh);
        has_rbf_mesh_ = true;
    }

    current_mesh_idx_ = static_cast<int>(mesh_.size()) - 1;

    for(int i = 0; i < static_cast<int>(mesh_.size()); ++ i){
        mesh_[i].update_face_normals();
        mesh_[i].update_vertex_normals();

        indices_.push_back(std::vector<unsigned int>());
        update_face_indices(mesh_[i], indices_.back());
    }

    set_scene();
}


//-----------------------------------------------------------------------------


ReconstructionViewer::
~ReconstructionViewer()
{
}

//-----------------------------------------------------------------------------


void
ReconstructionViewer::
keyboard(int key, int x, int y)
{
    switch (toupper(key))
    {
    case 'N':
        {
            if(!normal_end_points_.empty())
            {
                draw_normals_ = !draw_normals_;
                std::cout << "Display of normals: ";
                if(draw_normals_){
                    std::cout << "ON";
                }
                else{
                    std::cout << "OFF";
                }
                std::cout << std::endl << std::flush;
                glutPostRedisplay();
            }
            break;
        }
    case 'P':
        {
            if(!points_.empty())
            {
                draw_points_ = !draw_points_;
                std::cout << "Display of points: ";
                if(draw_points_){
                    std::cout << "ON";
                }
                else{
                    std::cout << "OFF";
                }
                std::cout << std::endl << std::flush;
                glutPostRedisplay();
            }
            break;
        }
    case 'M':
        {
            if(!mesh_.empty())
            {
                current_mesh_idx_ = (current_mesh_idx_ + 1) % static_cast<int>(mesh_.size());

                std::cout << "Display mesh: ";

                if(static_cast<int>(mesh_.size()) == 1){
                    if(this->has_hoppe_mesh_){
                        std::cout << "Hoppe";
                    }
                    else{
                        std::cout << "RBF";
                    }
                }
                else{
                    if(current_mesh_idx_ == 1){
                        std::cout << "RBF";
                    }
                    else{
                        std::cout << "Hoppe";
                    }
                }
                std::cout << std::endl << std::flush;
                glutPostRedisplay();
            }
            break;
        }
    default:
        {
            GlutExaminer::keyboard(key, x, y);
            break;
        }
    }
}

//-----------------------------------------------------------------------------

void
ReconstructionViewer::
set_scene()
{
    if(points_.empty() && mesh_.empty()){
        GlutExaminer::set_scene(Vec3f(0.0f), 1.0f);
    }
    else{
        Vec3f bbMin(std::numeric_limits<surface_mesh::Scalar>::max()),
                bbMax(-std::numeric_limits<surface_mesh::Scalar>::max());

        for(int i = 0; i < static_cast<int>(points_.size()); ++i){
            bbMin.minimize(points_[i]);
            bbMax.maximize(points_[i]);
        }

        for(int i = 0; i < static_cast<int>(mesh_.size()); ++i){
            Mesh::Vertex_iterator  v_it(mesh_[i].vertices_begin()),
                                   v_end(mesh_[i].vertices_end());

            for (; v_it!=v_end; ++v_it)
            {
              bbMin.minimize(mesh_[i].position(*v_it));
              bbMax.maximize(mesh_[i].position(*v_it));
            }
        }

        GlutExaminer::set_scene((bbMin + bbMax)*0.5, 0.5* norm(bbMin - bbMax));
    }
}


//-----------------------------------------------------------------------------


void
ReconstructionViewer::
update_face_indices(surface_mesh::Surface_mesh &mesh, std::vector<unsigned int> &indices)
{
    Mesh::Face_iterator        f_it(mesh.faces_begin()),
                             f_end(mesh.faces_end());
    Mesh::Vertex_around_face_circulator  fv_c;

    indices.clear();
    indices.reserve(mesh.n_faces()*3);

    for (; f_it!=f_end; ++f_it)
    {
        fv_c=mesh.vertices(*f_it);
        indices.push_back((*fv_c).idx());

        ++ fv_c;
        indices.push_back((*fv_c).idx());

        ++ fv_c;
        indices.push_back((*fv_c).idx());
    }
}


//-----------------------------------------------------------------------------


void 
ReconstructionViewer::
draw(const std::string& _draw_mode)
{
    if(indices_.empty() && !draw_points_){
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

        glDrawElements(GL_TRIANGLES, get_current_face_indices().size(), GL_UNSIGNED_INT, &(get_current_face_indices()[0]));

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
        glDrawElements(GL_TRIANGLES, get_current_face_indices().size(), GL_UNSIGNED_INT, &(get_current_face_indices()[0]));
        glDisableClientState(GL_VERTEX_ARRAY);
        glColor3f(1.0, 1.0, 1.0);

        glEnableClientState(GL_VERTEX_ARRAY);
        GL::glVertexPointer(vertex_positions_pointer());

        glDrawBuffer(GL_BACK);
        glDepthRange(0.0, 1.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDepthFunc(GL_LEQUAL);
        glDrawElements(GL_TRIANGLES, get_current_face_indices().size(), GL_UNSIGNED_INT, &(get_current_face_indices()[0]));

        glDisableClientState(GL_VERTEX_ARRAY);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDepthFunc(GL_LESS);
    }

    else if (_draw_mode == "Solid Flat")
    {
        Mesh &mesh = get_current_mesh();
        Mesh::Face_iterator   f_it(mesh.faces_begin()), f_end(mesh.faces_end());
        Mesh::Vertex_around_face_circulator  fv_c;

        glEnable(GL_LIGHTING);
        glShadeModel(GL_FLAT);

        glBegin(GL_TRIANGLES);
        for (; f_it!=f_end; ++f_it)
        {
            GL::glNormal(face_normals()[*f_it]);

            fv_c = mesh.vertices(*f_it);
            GL::glVertex(mesh.position(*fv_c));

            ++fv_c;
            GL::glVertex(mesh.position(*fv_c));

            ++fv_c;
            GL::glVertex(mesh.position(*fv_c));
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

        glDrawElements(GL_TRIANGLES, get_current_face_indices().size(), GL_UNSIGNED_INT, &(get_current_face_indices()[0]));

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
    }

    if(_draw_mode == "Points" || this->draw_points_){
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glShadeModel(GL_SMOOTH);

        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);
        glColor3f(0.0, 0.5, 1.0);
        for(int i = 0; i < static_cast<int>(points_.size()); ++ i)
        {
         glPushMatrix();
         Vec3 &pt = points_[i];
         glTranslatef( pt[0], pt[1], pt[2] );
         glutSolidSphere( point_radius_, 10, 10 );
         glPopMatrix();
        }

        glDisable(GL_COLOR_MATERIAL);

        if(draw_normals_)
        {
            glDisable(GL_LIGHTING);
            glLineWidth(point_radius_ * 0.5);
            glColor3f(1.0, 1.0, 0.0);
            glBegin(GL_LINES);
            for(int i = 0; i < static_cast<int>(points_.size()); ++ i)
            {
                Vec3 &start_pt = points_[i], &end_pt = normal_end_points_[i];
                glVertex3f(start_pt[0], start_pt[1], start_pt[2]);
                glVertex3f(end_pt[0], end_pt[1], end_pt[2]);
            }
            glEnd();
        }

        glPopAttrib();
    }
}


//=============================================================================


Surface_mesh::Vertex_property<Normal>
ReconstructionViewer::
vertex_normals()
{
    return get_current_mesh().vertex_property<Normal>("v:normal");
}


//=============================================================================


Surface_mesh::Face_property<Normal>
ReconstructionViewer::
face_normals()
{
    return get_current_mesh().face_property<Normal>("f:normal");
}


//=============================================================================


Point*
ReconstructionViewer::
vertex_positions_pointer()
{
    return &(get_current_mesh().points()[0]);
}


//=============================================================================


Normal*
ReconstructionViewer::
vertex_normals_pointer()
{
    return &(vertex_normals().vector()[0]);
}


//=============================================================================
