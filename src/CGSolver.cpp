//
// Created by Luwei Yang on 6/11/15.
//

#include <LinearSolver.h>

using namespace Eigen;

template <typename T>
int CGSolver<T>::solve(const SparseMatrix<T> &A, const VectorXd &b, VectorXd &x) {

    ConjugateGradient<SparseMatrix<T> > cg;
    cg.compute(A);
    x = cg.solve(b);

    // Temporarily can not got any error
    return 0;
}

template class CGSolver<double>;