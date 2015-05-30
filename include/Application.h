/*
 * Spectral Conformal Parameterization
 * Patrick Mullen, Yiying Tong, Pierre Alliez, Mathieu Desbrun.
 * Symposium of Geometry Processing, 2008.
 */

#ifndef DDG_APPLICATION_H
#define DDG_APPLICATION_H

#include "Mesh.h"
#include "Complex.h"
#include "DenseMatrix.h"
#include "SparseMatrix.h"
#include "DiscreteExteriorCalculus.h"


//template class Polynomial<2>;
namespace DDG
{
    class Application
    {
    public:
        void run(Mesh& mesh, Octree &octree){
            // Test
            octree.generateOctreeFrom(mesh, 5);
        }
        
    };
}

#endif
