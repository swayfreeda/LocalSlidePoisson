//
// Created by sway on 6/16/15.
//

#include "PoissonSurfaceReconstruction.h"


/*
 * FunctionTree
 */
void FunctionNode::addChildren() {
    
    if (left_child_ != NULL ||
        right_child_ != NULL)
        return;
    
    left_child_ = new FunctionNode();
    left_child_->setParent(dynamic_cast<BinaryNode*>(this));
    left_child_->setDepth(this->depth_ + 1);
    
    right_child_ = new FunctionNode();
    right_child_->setParent(dynamic_cast<BinaryNode*>(this));
    right_child_->setDepth(this->depth_ + 1);
}

FunctionNode* FunctionNode::leftChild()
{
    return dynamic_cast<FunctionNode*>(left_child_);
}


DDG::Mesh *PoissonRec::Pipeline(const DDG::Mesh *input_mesh) {
//
//    // Step 1:  build Octree
//    buildOctree(input_mesh);
//
//    // Step 2:  build function tree
//    buildFunctionTree();
//
//    // Step 3:  build vector filed
//    buildVectorField();
//
//    // Step 4:  build Laplacian matrix and right part b
//    buildTables();
//    Eigen::VectorXd b = buildRightPart();
//    Eigen::SparseMatrix<double> L = buildLaplacianMatrix();
//
//    // Step 5:  optimization
//    Eigen::VectorXd x;
//    CGSolver<double>::Solve(L, b, x);
//
//    // Step 6:  assign a function values for octree cell corner
//    updateCellCornerValues(x);
//
//    // Step 7:  extract mesh via marching cube
//    DDG::Mesh *mesh = IsoSurfaceExtractor::ExtractOctree(octree);
//
//    return mesh;
    
    return NULL;
}

void PoissonRec::buildTables() {
//    buildDotTable();
//    buildDDotTable();
//    buildD2DotTable();
}
