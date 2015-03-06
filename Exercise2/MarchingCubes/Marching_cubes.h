//=============================================================================


#ifndef MARCHING_CUBES_H
#define MARCHING_CUBES_H


//== INCLUDES =================================================================


#include "Grid.h"
#include <surface_mesh/Surface_mesh.h>
#include <map>


//=============================================================================


void marching_cubes(const Grid& _grid, Surface_mesh& _mesh, Scalar isoval=0);


//-----------------------------------------------------------------------------


class Marching_cubes
{
public:

    Marching_cubes(const Grid& _grid, Surface_mesh& _mesh, Scalar isoval=0);


private:

    void process_cube(unsigned int x, unsigned int y, unsigned int z);
    Surface_mesh::Vertex add_vertex(const Vec3ui& p0, const Vec3ui& p1);


private:

    const Grid&     grid_;
    Surface_mesh&   mesh_;
    Scalar          isoval_;
    std::map<unsigned long int, Surface_mesh::Vertex> edge2vertex_;

    static int edgeTable[256];
    static int triTable[256][17];
};


//=============================================================================
#endif
//=============================================================================
