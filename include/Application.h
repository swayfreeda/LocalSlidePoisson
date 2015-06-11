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
#include "IsoSurfaceExtractor.h"
#include <LinearSolver.h>

#undef complex
//template class Polynomial<2>;
namespace DDG
{
    class Application
    {
    public:
        void run(Mesh *mesh, Octree &octree) {
            // Test
            //octree.generateOctreeFrom(mesh, 5);

            octree.generateOctreeFrom(mesh, 4);

            // TEST: iterator
            long long nodeCounts = 0;

            for (Octree::CellIterator itr = octree.begin();
                 itr != octree.end();
                 itr++) {

                nodeCounts++;
                OctreeCell *cell = *(itr);
                int depth = cell->getDepth();
                printf("Depth: %d", depth);

            }
            //int nodeCounts = octree.nodeCounts();
            
            // TEST: Deallocate the octree
            //octree.deallocate();

            // TEST: Solve matrix via CG
            Eigen::SparseMatrix<double> A(3,3);
            Eigen::VectorXd b(3), x(3);

            A.insert(0, 0) = 3;
            A.insert(0, 1) = 2;
            A.insert(1, 0) = 1;
            A.insert(2, 1) = 2;
            A.insert(2, 2) = 1;

            b[0] = 1;
            b[1] = 2;
            b[2] = 3;

            CGSolver<double>::solve(A,b,x);
            cout<<endl;
            cout<<"A: "<<A<<endl;
            cout<<"x: "<<x<<endl;
        }
        
    };
}

#endif
