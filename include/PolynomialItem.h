//
// Created by sway on 5/30/15.
//

#ifndef RECONSTRUCTOR_POLYNOMIALITEM_H
#define RECONSTRUCTOR_POLYNOMIALITEM_H

template <int Degree>
class PolynomialItem{
public:
    //constructor
    PolynomialItem(void);

    template<int Degree2>
    PolynomialItem<Degree>(const PolynomialItem<Degree2>&P);

    template<int Degree2>
    PolynomialItem<Degree> &operator=(const  PolynomialItem<Degree2>&p);

    // get the value of the polynomial
    double operator()(double x) const;

    //integral of a polynomial
    PolynomialItem<Degree +1> integral(void) const;

    //derivative of a polynomial
    PolynomialItem<Degree-1> derivative(void) const;

    // integral between a range
    double integral(double min, double max);

    // scale of a polynomial
    PolynomialItem<Degree> scale(const double &s) const;

    // shift of a polynomial
    PolynomialItem<Degree> shift(const double &t) const;

    template<int Degree2>
    PolynomialItem <Degree + Degree2> operator * (const PolynomialItem<Degree2> & p)const;

    PolynomialItem <Degree>&operator-=(const double s);
    PolynomialItem <Degree>&operator+=(const double s);
    PolynomialItem <Degree>&operator*=(const double s);
    PolynomialItem <Degree>&operator/=(const double s);
    PolynomialItem<Degree> operator-(const double s) const;
    PolynomialItem<Degree> operator+(const double s) const;
    PolynomialItem<Degree> operator*(const double s) const;
    PolynomialItem<Degree> operator/(const double s) const;

    PolynomialItem<Degree> operator+( const PolynomialItem<Degree>& p ) const;
    PolynomialItem<Degree> operator-( const PolynomialItem<Degree>& p ) const;
    PolynomialItem<Degree>& operator +=(const PolynomialItem<Degree>& p);
    PolynomialItem<Degree>& operator -=(const PolynomialItem<Degree>& p);
    PolynomialItem<Degree> operator-( void ) const;

    // printf the information of a polynomial
    void printnl(void) const;

public:
    double coefficients[Degree+1];
};


template <int Degree>
class StartingPolynomialItem{
public:

    //StartingPolynomialItem (const StartingPolynomialItem<Degree> &sp);

    template<int Degree2>
    StartingPolynomialItem<Degree + Degree2> operator *(const StartingPolynomialItem<Degree2> &sp)const;

    StartingPolynomialItem<Degree>scale(const double &s) const;

    StartingPolynomialItem<Degree> shift(const double &t) const;

    int operator < (const StartingPolynomialItem <Degree>&sp) const;

    static int Compare(const void *v1, const void *v2);

public:
    double start;
    PolynomialItem<Degree> p;
};
#include"PolynomialItem.inl"
#endif //RECONSTRUCTOR_POLYNOMIALITEM_H
