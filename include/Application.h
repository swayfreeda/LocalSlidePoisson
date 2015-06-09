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
        void run(Mesh *mesh, Octree &octree) {
            // Test
            octree.generateOctreeFrom(mesh, 5);
            
            // TEST: iterator
//            long long nodeCounts = 0;
//            Octree::CellIterator itr;
//            for (itr = octree.begin();
//                 itr != octree.end();
//                 itr++) {
//
//                nodeCounts++;
//                OctreeCell *cell = *(itr);
//                int depth = cell->getDepth();
//                printf("Depth: %d", depth);
//
//            }
            int nodeCounts = octree.nodeCounts();
            
            // TEST: Deallocate the octree
            //octree.deallocate();


        }
        
    };
}

#endif
