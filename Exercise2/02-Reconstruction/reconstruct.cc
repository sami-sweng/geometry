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

#include "ImplicitHoppe.hh"
#include "ImplicitRBF.hh"
#include "ReconstructionViewer.hh"
#include "MarchingCubes/Marching_cubes.h"
#include "surface_mesh/IO.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <cmath>

#define DISPLAY_HOPPE 0
#define MC_RESOLUTION 51


bool str2int (int &i, char const *s)
{
    char *end;
    long  l;
    errno = 0;
    l = strtol(s, &end, 0);
    if ((errno == ERANGE && l == LONG_MAX) || l > INT_MAX) {
        return false;
    }
    if ((errno == ERANGE && l == LONG_MIN) || l < INT_MIN) {
        return false;
    }
    if (*s == '\0' || *end != '\0') {
        return false;
    }

    i = l;
    return true;
}

//=============================================================================


int main(int argc, char **argv)
{
	// parse command line
	if (argc<2)
	{
        std::cerr << "Usage:" << std::endl
        << argv[0] << "  <input-points>  <neighborhood-size> [<output-mesh>]" << std::endl;
		exit(1);
	}

    bool do_normal_fitting = false;
    int neighborhood_size = -1;
    if(argc >= 3){
        if(!str2int(neighborhood_size, argv[2])){
            std::cerr << "Error: unable to convert the second argument into neighborhood size" << std::endl;
            exit(1);
        }
        else{
            if(neighborhood_size < 3){
                std::cerr << "Error: neighborhood_size < 3" << std::endl;
                exit(1);
            }
        }
        do_normal_fitting = true;
    }

    bool do_rbf_fitting = false;
    if(argc >= 4){
        std::ofstream ofile(argv[3]);

        if(!ofile.is_open()){
            std::cerr << "Unable to open the output mesh file" << std::endl;
            exit(1);
        }

        ofile.close();
        do_rbf_fitting = true;
    }

    // read point clouds
    typedef surface_mesh::Surface_mesh Mesh;
    Mesh pointcloud_mesh;
    std::cout << "Reading point cloud file...." << std::endl;
    if(!surface_mesh::read_mesh(pointcloud_mesh, argv[1]))
    {
      std::cerr << "Error: unable to read the input point cloud file." << std::endl;
      exit(1);
    }

    std::vector<surface_mesh::Vec3> points;
    Mesh::Vertex_iterator v_it, v_end(pointcloud_mesh.vertices_end());
    for(v_it = pointcloud_mesh.vertices_begin(); v_it != v_end; ++ v_it){
        points.push_back(pointcloud_mesh.position(*v_it));
    }

    ImplicitHoppe *hoppe_fitting = NULL;
    ImplicitRBF *rbf_fitting = NULL;

    Mesh hoppe_mesh, rbf_mesh;
    std::vector<surface_mesh::Vec3> normals;

    if(do_normal_fitting && !points.empty()){
        std::cout << "Computing point cloud normals...." << std::endl;
        hoppe_fitting = new ImplicitHoppe(points, neighborhood_size);
        normals = hoppe_fitting->get_normals();

        // Compute the bounding box
        surface_mesh::Vec3 bb_min = points[0], bb_max = points[0];
        for(int i = 1; i < static_cast<int>(points.size()); ++i)
        {
            bb_min.minimize( points[i] );
            bb_max.maximize( points[i] );
        }

        surface_mesh::Vec3 bb_center = (bb_max + bb_min)*0.5f;

        // Determine a suitable grid size, by sligting enlarging the bounding box
        surface_mesh::Vec3 bb_diag = (bb_max - bb_min) * 1.2;
        double bb_size = std::max( bb_diag[0], std::max(bb_diag[1], bb_diag[2]) );
        double grid_size = bb_size / (MC_RESOLUTION - 1);

        unsigned int resolution[3];
        for(int i = 0; i < 3; ++i){
            resolution[i] = std::ceil(bb_diag[i] / grid_size) + 1;
        }

        for(int i = 0; i < 3; ++ i){
            bb_min[i] = bb_center[i] - grid_size * (resolution[i] - 1) * 0.5;
        }

        bb_max = bb_center * 2.0 - bb_min;
        bb_diag = bb_max - bb_min;

        // slightly enlarge bounding box
        // turn bounding box into a bounding cube (same edge lengths)
        //const Scalar factor = 0.6;
        //bb_min = bb_center - factor * surface_mesh::Vec3(bb_size);
        //bb_max = bb_center + factor * surface_mesh::Vec3(bb_size);



        // Construct the Marching Cubes grid
        Grid *grid = NULL;

        if(DISPLAY_HOPPE || do_rbf_fitting)
        {
            grid = new Grid(bb_min,
                            Point(bb_diag[0], 0, 0),
                            Point(0, bb_diag[1], 0),
                            Point(0, 0, bb_diag[2]),
                            resolution[0], resolution[1], resolution[2]);
        }

        if(DISPLAY_HOPPE){
            std::cout << "Surface reconstruction using the Hoppe method..." << std::endl;

            for (unsigned int x=0; x<resolution[0]; ++x)
                for (unsigned int y=0; y<resolution[1]; ++y)
                    for (unsigned int z=0; z<resolution[2]; ++z)
                        (*grid)(x,y,z) = (*hoppe_fitting)( grid->point(x,y,z) );

            std::cout << "Marching cubes for Hoppe fitting surface..." << std::endl;

            Marching_cubes(*grid, hoppe_mesh);

            std::cout << "Done: "
                      << hoppe_mesh.n_vertices() << " vertices, "
                      << hoppe_mesh.n_faces() << " faces" << std::endl;
        }

        if(do_rbf_fitting){
            std::cout << "Surface reconstruction using triharmonic RBF..." << std::endl;

            rbf_fitting = new ImplicitRBF(points, hoppe_fitting->get_normals());

            std::cout << "Marching cubes for RBF fitting surface..." << std::endl;

            for (unsigned int x=0; x<resolution[0]; ++x)
                for (unsigned int y=0; y<resolution[1]; ++y)
                    for (unsigned int z=0; z<resolution[2]; ++z)
                        (*grid)(x,y,z) = (*rbf_fitting)( grid->point(x,y,z) );

            Marching_cubes(*grid, rbf_mesh);

            std::cout << "Done: "
                      << rbf_mesh.n_vertices() << " vertices, "
                      << rbf_mesh.n_faces() << " faces" << std::endl;
        }

        if(grid){
            delete grid;
        }
    }

    if(do_rbf_fitting){
        std::cout << "Writing RBF mesh..." << std::endl << std::flush;
        if (!surface_mesh::write_mesh(rbf_mesh, argv[3]))
        {
            std::cerr << "Error: unable to write mesh." << std::endl;
            exit(1);
        }
    }

    // Display results
    glutInit(&argc, argv);


    ReconstructionViewer window("Reconstruction Viewer", 512, 512,
                                &points,
                                points.size() > 2 ? PointCloudNormals::average_point_distance(points) : 0.1,
                                normals.empty() ? NULL : (&normals),
                                hoppe_mesh.n_vertices() > 0 ? (&hoppe_mesh) : NULL,
                                rbf_mesh.n_vertices() > 0 ? (&rbf_mesh) : NULL);

    if(hoppe_fitting){
        delete hoppe_fitting;
    }

    if(rbf_fitting){
        delete rbf_fitting;
    }

    glutMainLoop();

}

//=============================================================================
