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
//  CLASS SmoothingViewer - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

#include "SmoothingViewer.hh"
#include <fstream>
#include <iostream>



//== IMPLEMENTATION ========================================================== 


SmoothingViewer::
SmoothingViewer(const char* _title, int _width, int _height)
  : QualityViewer(_title, _width, _height),
    enhancement_smoothing_iterations_(1),
    enhancement_coef_(1.0f),
    init_success_(false)
{
    init_success_ = load_setting_file();
}


bool
SmoothingViewer::
open_mesh(const char* _filename)
{
    // load mesh
    if (QualityViewer::open_mesh(_filename))
    {
        v_new_pos_ = mesh_.vertex_property<Point>("v:NewPos");
        v_old_pos_ = mesh_.vertex_property<Point>("v:OldPos");
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------


void
SmoothingViewer::
keyboard(int key, int x, int y)
{
	switch (toupper(key))
	{
	case 'N':
		{
			std::cout << "10 Laplace-Beltrami smoothing iterations: " << std::flush;
            smooth(10);
			calc_weights();
			calc_mean_curvature();
			calc_uniform_mean_curvature();
			calc_gauss_curvature();
			calc_triangle_quality();
			face_color_coding();

			glutPostRedisplay();
			std::cout << "done\n";
			break;
		}
	case 'U':
		{
			std::cout << "10 uniform smoothing iterations: " << std::flush;
            uniform_smooth(10);
			calc_weights();
			calc_mean_curvature();
			calc_uniform_mean_curvature();
			calc_gauss_curvature();
			calc_triangle_quality();
			face_color_coding();

			glutPostRedisplay();
			std::cout << "done\n";
			break;
		}
    case 'K':
        {
            if(init_success_){
                std::cout << "Uniform Laplacian feature enhancement: " << std::flush;
                uniform_laplacian_enhance_feature();
                calc_weights();
                calc_mean_curvature();
                calc_uniform_mean_curvature();
                calc_gauss_curvature();
                calc_triangle_quality();
                face_color_coding();

                glutPostRedisplay();
                std::cout << "done\n";
            }
            else{
                std::cerr << "Error: unable to performa feature enhancement, because the program failed ot load the setting file" << std::endl;
            }
            break;
        }
    case 'L':
        {
            if(init_success_){
                std::cout << "Laplace-Beltrami feature enhancement: " << std::flush;
                laplace_beltrami_enhance_feature();
                calc_weights();
                calc_mean_curvature();
                calc_uniform_mean_curvature();
                calc_gauss_curvature();
                calc_triangle_quality();
                face_color_coding();

                glutPostRedisplay();
                std::cout << "done\n";
            }
            else{
                std::cerr << "Error: unable to performa feature enhancement, because the program failed ot load the setting file" << std::endl;
            }
            break;
        }
    case 'G':
        {
            std::cout << "1 iteartions of uniform smoothing: " << std::flush;
            uniform_smooth(1);
            calc_weights();
            calc_mean_curvature();
            calc_uniform_mean_curvature();
            calc_gauss_curvature();
            calc_triangle_quality();
            face_color_coding();

            glutPostRedisplay();
            std::cout << "done\n";
            break;
        }

	default:
		{
			QualityViewer::keyboard(key, x, y);
			break;
		}
	}
}


//-----------------------------------------------------------------------------


void 
SmoothingViewer::
smooth(unsigned int _iters)
{
    Mesh::Vertex_iterator   v_it, v_end(mesh_.vertices_end());
    Mesh::Halfedge    h;
    Mesh::Edge        e;
    Mesh::Halfedge_around_vertex_circulator  vh_c, vh_end;
    Mesh::Vertex      neighbor_v;
    Point             laplace;
    Scalar            w, ww;

    for (unsigned int iter=0; iter<_iters; ++iter)
    {
        // INSERT CODE:
        // Perform Laplace-Beltrami smoothing:
        // 1) precompute edge weights using calc_edge_weights()
        calc_edge_weights();

        // 2) for each non-boundary vertex, update its position using the normalized Laplace-Beltrami operator
        //    (Hint: use the precomputed edge weights in eweight_)
        for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
        {
            if (!mesh_.is_boundary(*v_it))
            {
                laplace = Point(0.0);

                vh_c = mesh_.halfedges(*v_it);
                vh_end = vh_c;

                ww=0;

                do{
                    e = mesh_.edge(*vh_c);
                    neighbor_v = mesh_.to_vertex(*vh_c);
                    w=eweight_[e];
                    laplace += w * (mesh_.position(neighbor_v) - mesh_.position(*v_it));
                    ww+=w;

                }while(++vh_c != vh_end);

                laplace /= ww;

                mesh_.position(*v_it) += 0.5*laplace;
            }

        }

    }

	// update face and vertex normals
    mesh_.update_face_normals();
    mesh_.update_vertex_normals();
}

//-----------------------------------------------------------------------------


void 
SmoothingViewer::
uniform_smooth(unsigned int _iters)
{
    Mesh::Vertex_iterator   v_it, v_end(mesh_.vertices_end());
    Mesh::Vertex_around_vertex_circulator  vv_c, vv_end;
    Point             laplacian;
    unsigned int            w;

	for (unsigned int iter=0; iter<_iters; ++iter)
	{
        // INSERT CODE:
        // For each non-boundary vertex, update its position according to the uniform Laplacian operator
        for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
        {
            if (!mesh_.is_boundary(*v_it))
            {
                laplacian = Point(0.0);
                double n = 0;
                vv_c = mesh_.vertices(*v_it);
                vv_end = vv_c;

                do{
                    laplacian += mesh_.position(*vv_c);
                    ++n;
                }while(++vv_c != vv_end);

                laplacian /= n;
                laplacian -= mesh_.position(*v_it);

                mesh_.position(*v_it) += 0.5*laplacian;

            }
        }
	}


	// update face and vertex normals
    mesh_.update_face_normals();
    mesh_.update_vertex_normals();
}


//=============================================================================

bool
SmoothingViewer::
load_setting_file()
{
    std::ifstream in_file("./Setting.txt");

    std::cout << "Reading the parameter file..." << std::endl;

    if (!in_file.is_open())
    {
        std::cerr << "Unable to open the setting file" << std::endl;
        return false;
    }

    in_file >> enhancement_smoothing_iterations_ >> enhancement_coef_;

    if(in_file.fail()){
        std::cerr << "Unable to read parameters from the setting file" << std::endl;
        return false;
    }

    if(enhancement_smoothing_iterations_ < 1){
        std::cerr << "Invalid nmber of smoothing iterations for feature enhancement" << std::endl;
        return false;
    }

    std::cout << "Itearations of smoothing for feature enhancement: " << enhancement_smoothing_iterations_ << std::endl;
    std::cout << "Feature enhancement coefficient: " << enhancement_coef_ << std::endl;

    return true;
}

//=============================================================================


void
SmoothingViewer::
uniform_laplacian_enhance_feature()
{
    Mesh::Vertex_iterator   v_it, v_end(mesh_.vertices_end());
    Point *old_mesh = new Point[mesh_.n_vertices()];
    int k=0;

    //copy the mesh
    for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
    {
        old_mesh[k]=Vec3(mesh_.position(*v_it));
        k++;
    }

    // INSERT CODE:
    // Feature enhancement using the uniform Laplacian operator:
    // 1) perform uniform Laplacian smoothing for enhancement_smoothing_iterations_ iterations
    uniform_smooth(enhancement_smoothing_iterations_);

    // 2) update the vertex positions according to the difference between the original and the smoothed mesh,
    //    using enhancement_coef_ as the value of alpha in the feature enhancement formula
    Scalar alpha=enhancement_coef_;
    k=0;
    for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
    {
        mesh_.position(*v_it)=alpha*old_mesh[k] + (1-alpha)*mesh_.position(*v_it);
        k++;
    }


    mesh_.update_face_normals();
    mesh_.update_vertex_normals();
}


//=============================================================================

void
SmoothingViewer::
laplace_beltrami_enhance_feature()
{
    Mesh::Vertex_iterator   v_it, v_end(mesh_.vertices_end());
    Point *old_mesh = new Point[mesh_.n_vertices()];
    int k=0;

    //copy the mesh
    for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
    {
        old_mesh[k]=Vec3(mesh_.position(*v_it));
        k++;
    }

    // INSERT CODE:
    // Feature enhancement using the Laplace-Beltrami operator:
    // 1) perform Laplace-Beltrami smoothing for enhancement_smoothing_iterations_ iterations
    smooth(enhancement_smoothing_iterations_);

    // 2) update the vertex positions according to the difference between the original and the smoothed mesh,
    //    using enhancement_coef_ as the value of alpha in the feature enhancement formula
    Scalar alpha=enhancement_coef_;
    k=0;
    for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
    {
        mesh_.position(*v_it)=alpha*old_mesh[k] + (1-alpha)*mesh_.position(*v_it);
        k++;
    }

    mesh_.update_face_normals();
    mesh_.update_vertex_normals();
}
