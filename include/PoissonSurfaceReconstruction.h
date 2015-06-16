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
   Polynomial<3> guassian_fun;
   Polynomial<3> dguassian_fun;

    // reload the child from base class
    void addChildren();
    
    FunctionNode *leftChild();
    FunctionNode *rightChild();
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
