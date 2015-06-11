//
// Created by Luwei Yang on 6/11/15.
//

#ifndef RECONSTRUCTOR_LINEARSOLVER_H
#define RECONSTRUCTOR_LINEARSOLVER_H

#include <Eigen/Sparse>
#include <Eigen/Dense>

using namespace Eigen;

// SolverType
enum SolverType
{
    kLUDecomposition = 0,
    kQRDecomposition,
    kCholeskyDecomposition,
    kConjugateGradient
};

enum ErrorType
{
    kDecompositionError = 0,
    kSolveError,
    kRankEfficientError,
    kMatrixSingularError,
};

template <class T>
class LinearSolver
{
public:
    // Solve Ax = b
    //static int solve(const SparseMatrix<T>& A, const Matrix<T, Dynamic, 1>  &b, Matrix<T, Dynamic, 1> &x);
};


#endif //RECONSTRUCTOR_LINEARSOLVER_H
