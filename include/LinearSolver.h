//
// Created by Luwei Yang on 6/11/15.
//

#ifndef RECONSTRUCTOR_LINEARSOLVER_H
#define RECONSTRUCTOR_LINEARSOLVER_H

#include <Eigen/Sparse>
#include <Eigen/Dense>


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

template <typename T>
class LinearSolver
{
public:
    // Solve Ax = b
    // static int solve(const SparseMatrix<T>& A, const Matrix<T, Eigen::Dynamic, 1>  &b, Matrix<T, Eigen::Dynamic, 1> &x);
};

template <typename T>
class CGSolver:public LinearSolver<T>
{
public:
    // Solve Ax = b via interation
    static int solve(const Eigen::SparseMatrix<T> &A, const Eigen::VectorXd &b, Eigen::VectorXd &x);

    // Solve Ax = b via iteration with external parameter of max iteration
    // and torlerance
    //static int solve(const Eigen::SparseMatrix<T> &A, const Eigen::Matrix<T, Eigen::Dynamic, 1> &b, Eigen::Matrix<T, Eigen::Dynamic, 1> &x,
    //              const int maxIteration, const double torlerance);
};


#endif //RECONSTRUCTOR_LINEARSOLVER_H
