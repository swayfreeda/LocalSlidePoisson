//
// Created by sway on 5/30/15.
//

#ifndef RECONSTRUCTOR_POLYNORMIAL_H
#define RECONSTRUCTOR_POLYNORMIAL_H

#include "PolynomialItem.h"

template<int Degree>
class Polynormial {
public:

    //constructor
    Polynormial(void);
    Polynormial(const Polynormial<Degree> &p);
    Polynormial& operator =(const Polynormial&p);
    //destroyer
    ~Polynormial(void);

    void set(int size);
    void set(StartingPolynormialItem<Degree>*sps, int &count);
    void reset(int newSize );


    static Polynormial GuassianApporximation(const double &width =0.5);
    Polynormial<Degree +1> MovingAverage(double radius);


public:
    int polyCount;
    StartingPolynormialItem<Degree> *polys;

};


#include "Polynormial.hpp"
#endif //RECONSTRUCTOR_POLYNORMIAL_H


