//
// Created by sway on 5/30/15.
//

#ifndef RECONSTRUCTOR_POLYNOMIAL_H
#define RECONSTRUCTOR_POLYNOMIAL_H

#include "PolynomialItem.h"
#include <stdlib.h>
#include <vector>

template<int Degree>
class Polynomial {
public:

    //constructor
    Polynomial(void);
    Polynomial(const Polynomial<Degree> &p);
    Polynomial& operator =(const Polynomial&p);
    //destroyer
    ~Polynomial(void);

    // initialize the polynomial
    void set(const int &size);
    void set(StartingPolynomialItem<Degree>*sps, const int &count);
    void reset(const int &newSize );

    //operator for scalar operation
    Polynomial& operator += (const double &s);
    Polynomial& operator -= (const double &s);
    Polynomial& operator *= (const double &s);
    Polynomial& operator /= (const double &s);

    Polynomial operator + (const double &s) const;
    Polynomial operator - (const double &s) const;
    Polynomial operator * (const double &s) const;
    Polynomial operator / (const double &s) const;

    //operator between two polynomials
    Polynomial  operator + (const Polynomial& p) const;
    Polynomial  operator - (const Polynomial& p) const;

    //scale and shift a polynomial
    Polynomial scale(const double &s) const;
    Polynomial shift(const double &t) const;

    // integral and derivative
    Polynomial<Degree -1> derivative(void) const;
    Polynomial<Degree +1> integral(void) const;
    double integral(const double& tMin,const double& tMax) const;
    double Integral(void) const;
    double operator()(const double& t) const;

    // approximating the Guassion funtion
    static Polynomial ConstantFunction(const double &width = 0.5);
    static Polynomial GuassianApproximation(const double &width =0.5);
    Polynomial<Degree +1> MovingAverage(const double &radius);

    // print the info of polynomial
    void printnl(void) const;


public:
    int polyCount;
    StartingPolynomialItem<Degree> *polys;

};
#include "Polynomial.inl"
#endif //RECONSTRUCTOR_POLYNOMIAL_H


