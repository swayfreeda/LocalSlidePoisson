//
// Created by Luwei Yang on 6/11/15.
//

#include <CGSolver.h>

template <class T>
int CGSolver::solve(const SparseMatrix<T> &A, const Matrix<T, Dynamic, 1> &b, Matrix<T, Dynamic, 1> &x) {

    ConjugateGradient<SparseMatrix<T> > cg;
    cg.compute(A);
    x = cg.solve(b);

    // Temporarily can not got any error
    return 0;
}

template <class T>
int CGSolver::solve(const SparseMatrix<T> &A, const Matrix<T, Dynamic, 1> &b, Matrix<T, Dynamic, 1> &x,
                    int maxIteration = 10000, double torlerance = 1e-6)
{

    ConjugateGradient<SparseMatrix<T> > cg;
    cg.setMaxIterations(maxIteration);
    cg.setTolerance(torlerance);

    cg.compute(A);
    x = cg.solve(b);

    return 0;
}