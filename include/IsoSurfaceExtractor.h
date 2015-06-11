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
     static int edgeTable[256];

     // Marching Cube's Triangle Table
     static int triangleTable[256][16];

     // Intepolate method for Marching Cube
     static Vector3D intepolate(Vector3D position1, Vector3D position2,
                         double position1_value, double position2_value)
     {return (position1 + (-position1_value / (position2_value - position1_value)) * (position2 - position1));}


public:

     // Extract mesh from voxel, the parameter is a function ptr
     // Note that the input of the function ptr must be f(x,y,z)
     static DDG::Mesh* extractVoxel(Vector3D start, Vector3D end, double strideWidth, double (*func)(double, double, double));



};


#endif //RECONSTRUCTOR_ISOSURFACEEXTRACTOR_H
