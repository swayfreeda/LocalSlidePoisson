//
// Created by sway on 6/16/15.
//

#include "PoissonSurfaceReconstruction.h"

DDG::Mesh *PoissonRec::Pipeline(const DDG::Mesh *input_mesh) {

    // Step 1:  build Octree
    buildOctree(input_mesh);

    // Step 2:  build function tree
    buildFunctionTree();

    // Step 3:  build vector filed
    buildVectorField();

    // Step 4:  build Laplacian matrix and right part b
    buildTables();
    Eigen::VectorXd b = buildRightPart();
    Eigen::SparseMatrix L = buildLaplacianMatrix();


    // Step 5:  optimization
    Eigen::VectorXd x;
    CGSolver::Solve(L, b, x);

    // Step 6:  assign a function values for octree cell corner
    updateCellCornerValues(x);

    // Step 7:  extract mesh via marching cube
    DDG::Mesh *mesh = IsoSurfaceExtractor::ExtractOctree(octree);

    return mesh;
}

void PoissonRec::buildTables() {
    buildDotTable();
    buildDDotTable();
    buildD2DotTable();
}
