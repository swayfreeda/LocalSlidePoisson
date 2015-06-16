//
// Created by sway on 6/16/15.
//

#ifndef RECONSTRUCTOR_POISSON_H
#define RECONSTRUCTOR_POISSON_H

#include <Eigen/Sparse>
#include <Eigen/Dense>
#include "OctreeCell.h"
#include "Polynomial.h"
#include "BinaryTree.h"
#include "IsoSurfaceExtractor.h"
#include "Octree.h"
#include "LinearSolver.h"


class FunctionNode: public BinaryNode
{
public:
   // gaussian function
   Polynomial guassian_fun;
   Polynomial dguassian_fun;

};

// class
class FunctionTree : public BinaryTree
{
public:
   void buildFunctionTree(int depth);

};


class PoissonRec{

private:

   // Dot tables
   double *dot_table;
   double *ddot_table;
   double *d2dot_table;

   FunctionTree *function_tree;
   Octree *octree;

   // Given two nodes global offset in a specific  dimension
   // return the dot value
   double dotValue(int offset1, int offset2);
   double dDotValue(int offset1, int offset2);
   double d2DotValue(int offset1, int offset2);


   void buildDotTable();
   void buildDDotTable();
   void buildD2DotTable();
   void buildTables();

   void buildOctree(const DDG::Mesh *mesh);
   void buildFunctionTree();
   void buildVectorField();
   Eigen::VectorXd buildRightPart();
   Eigen::SparseMatrix<double> buildLaplacianMatrix();

   // For marching cube, input vector is coefficients of
   // gaussian kernel functions
   void updateCellCornerValues(Eigen::VectorXd x);

public:
   PoissonRec(){}

   DDG::Mesh* Pipeline(const DDG::Mesh* input_mesh);


};

#endif //RECONSTRUCTOR_POISSON_H

DDG::Mesh* PoissonRec::Pipeline(const DDG::Mesh* input_mesh) {

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
