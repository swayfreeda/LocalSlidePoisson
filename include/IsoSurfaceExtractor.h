//
// Created by Luwei Yang on 6/9/15.
//

#ifndef RECONSTRUCTOR_ISOSURFACEEXTRACTOR_H
#define RECONSTRUCTOR_ISOSURFACEEXTRACTOR_H

#include "Mesh.h"
#include "Vector3D.h"

using namespace DDG;

class IsoSurfaceExtractor {
protected:
    
    // Marching Cube's Edge Table
    static int edge_table_[256];

    // Marching Cube's Triangle Table
    static int triangle_table_[256][16];

    // Intepolate method for Marching Cube
    static Vector3D intepolate(Vector3D position1, Vector3D position2,
                               double position1_value, double position2_value)
    {
        return (position1 + (-position1_value / (position2_value - position1_value)) * (position2 - position1));
    }


public:

    // Extract mesh from voxel, the parameter is a function ptr
    // Note that the input of the function ptr must be f(x,y,z)
    static DDG::Mesh* ExtractVoxel(Vector3D start,                             // Voxel start position (x,y,z)
                                   Vector3D end,                               // Voxel end position (x,y,z)
                                   double stride_width,                        // stride width for voxel
                                   double (*func)(double, double, double));    // indicator function pointer, input is (x,y,z)



};


#endif //RECONSTRUCTOR_ISOSURFACEEXTRACTOR_H
