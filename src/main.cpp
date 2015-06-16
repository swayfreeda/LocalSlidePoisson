//
//  main.cpp
//  Reconstructor
//
//  Created by Luwei Yang on 5/26/15.
//  Copyright (c) 2015 None. All rights reserved.
//

#include "Viewer.h"
#include "IsoSurfaceExtractor.h"

using namespace std;

static double Sphere(double x, double y, double z)
{
    return x * x + y * y + z * z - 1.0f;
}

static double CrazyFunction(double x, double y, double z)
{
    return sinf(x * y + x * z + y * z) + sinf(x * y) + sinf(y * z) + sinf(x * z) - 1.0f;
}

static double Heart(const double x,const double y, const double z)
{
    double r = (x*x + (9/4)*y*y + z*z - 1);
    return r*r*r - x*x*z*z*z - (9/80)*y*y*z*z*z;
}


using namespace DDG;

int main(int argc,char **argv)
{
    if( argc != 2 )  {
        cout << "usage: " << argv[0] << " in.obj" << endl;
        return 1;
    }
    
    Mesh *mesh = new Mesh();
    mesh->read(argv[1]);
    
//    Mesh *mesh2 = mesh->copyMesh();

    
    Viewer viewer;
    //viewer.mesh->read(argv[1]);
    
    const double SPAN = 2.0;
    viewer.mesh = mesh;
    
//     = IsoSurfaceExtractor::ExtractVoxel(Vector3D(-SPAN, -SPAN, -SPAN),   // Start
//                                                    Vector3D(SPAN, SPAN, SPAN),      // End
//                                                    0.05,                       // Stride
//                                                    Heart);                    // Function

    Mesh *cpmesh = viewer.mesh;
    viewer.init();


    
    return 0;
}
