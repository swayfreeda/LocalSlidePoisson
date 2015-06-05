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
            OctreeCell *cell = octree.rootNode()->getChildren(3)->getChildren(4)->getChildren(2);
//            cell->DB_flag =true;
//            //OctreeCell** neighbours = cell->neighbors();
//                for(int i=0; i< 3; i++){
//                    for(int j=0; j<3; j++)
//                        for(int k =0; k< 3; k++) {
//                            OctreeCell *neighbor = cell->neighbor(1, 1, 1);
//                            if (cell->neighbor(i, j, k))
//                                cell->neighbor(i, j, k)->DB_flag = true;
//                        }
//                }
        }
        
    };
}

#endif
