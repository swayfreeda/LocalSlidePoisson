//
//  main.cpp
//  Reconstructor
//
//  Created by Luwei Yang on 5/26/15.
//  Copyright (c) 2015 None. All rights reserved.
//
#include"Polynomial.h"

using namespace std;

#include "Viewer.h"
#include "IsoSurfaceExtractor.h"

#define SPAN 2.0

extern double Sphere(double x, double y, double z)
{
    return x * x + y * y + z * z - 1.0f;
}

extern double CrazyFunction(double x, double y, double z)
{
    return sinf(x * y + x * z + y * z) + sinf(x * y) + sinf(y * z) + sinf(x * z) - 1.0f;
}

extern double Heart(const double x,const double y, const double z)
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


    Viewer viewer;
    //viewer.mesh->read(argv[1]);
    viewer.mesh = IsoSurfaceExtractor::extractVoxel(Vector3D(-SPAN,-SPAN,-SPAN),   // Start
                                                    Vector3D(SPAN,SPAN,SPAN),      // End
                                                    0.05,                       // Stride
                                                    Heart);                    // Function
    viewer.init();

    return 0;
}