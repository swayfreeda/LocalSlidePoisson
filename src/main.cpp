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
using namespace DDG;

int main(int argc,char **argv)
{
    if( argc != 2 )  {
        cout << "usage: " << argv[0] << " in.obj" << endl;
        return 1;
    }

    Viewer viewer;
    viewer.mesh.read(argv[1]);
    viewer.init();

    return 0;
}