//
// Created by Luwei Yang on 5/27/15.
//

#include "PolynomialItem.h"

#include <stdlib.h>
#include <iostream>
#include <float.h>
//=====================================================Constructor====================================================//
template<int Degree>
PolynomialItem<Degree>::PolynomialItem(void){

    for(int i=0; i< Degree+1; i++) this->coefficients[i] =0;
}


//=====================================================Copy Constructor===============================================//
template<int Degree>
template <int Degree2>
PolynomialItem<Degree>::PolynomialItem(const PolynomialItem<Degree2>&p) {
    for (int i=0; i < Degree + 1; i++) coefficients[i] = 0;
    for (int i=0; i < Degree +1 && i<Degree2 +1; i++) coefficients[i] = p.coefficients[i];
}

//=====================================================Copy Constructor===============================================//
template<int Degree>
template <int Degree2>
PolynomialItem<Degree>& PolynomialItem<Degree>::operator=(const  PolynomialItem<Degree2>&p){
    for(int i=0; i< Degree +1; i++) this->coefficients[i] = 0;
    for (int i=0; i < Degree +1 && i<Degree2 +1; i++) coefficients[i] = p.coefficients[i];
    return *this;
}


//====================================================operator()======================================================//
template <int Degree>
double PolynomialItem<Degree>::operator()(double x) const {
    double temp = 1;
    double v = 0;
    for (int i = 0; i <Degree+1; i++) {
        v += temp * coefficients[i];
        temp *= x;
    }
    return v;
}

//====================================================integral========================================================//
template <int Degree>
PolynomialItem<Degree +1> PolynomialItem<Degree>::integral(void) const {
    PolynomialItem<Degree +1> p;
    p.coefficients[0] = 0;
    for(int i=0; i<Degree+1; i++){
      p.coefficients[i+1] = coefficients[i]/(i+1);
    }
   return p;
}

//====================================================derivate========================================================//
template <int Degree>
PolynomialItem<Degree-1> PolynomialItem<Degree>::derivative(void) const{
    PolynomialItem<Degree-1> p;
    for (int i = 0; i < Degree; i++)  {
        p.coefficients[i] = this->coefficients[i + 1] * (i + 1);
    }
    return p;

}

//====================================================internal========================================================//
template <int Degree>
double PolynomialItem<Degree>::integral(double min, double max){
    double v = 0;
    double t1, t2;
    t1 = min;
    t2 = max;

    for (int i = 0; i < Degree; i++)
    {
        v += coefficients[i] * (t2 - t1) / (i + 1);
        if (t1 != -DBL_MAX && t1 != DBL_MAX) { t1 *= min; }
        if (t2 != -DBL_MAX && t2 != DBL_MAX) { t2 *= max; }
    }

    return v;
}
//===================================================scale===========================================================//
template <int Degree>
PolynomialItem<Degree> PolynomialItem<Degree>:: scale(const double &s) const {

    PolynomialItem q = *this;
    double ss =1.0;
    for(int i=0; i< Degree +1; i++){
        q.coefficients[i]*=ss;
        ss/=s;
    }
   //q.printnl();
   return q;
}

//===================================================shift===========================================================//
template <int Degree>
PolynomialItem<Degree> PolynomialItem<Degree>:: shift(const double &t) const{
    PolynomialItem<Degree> q;
    for(int i=0; i< Degree +1; i++){
        double temp = 1;
        for(int j=i; j>=0; j--){
            q.coefficients[j] += coefficients[i] * temp;
            temp*=-t*j;
            temp/=(i-j+1);
        }
    }
    return q;
}
//===================================================printf===========================================================//
template <int Degree>
void  PolynomialItem<Degree>::printnl(void) const{
    for(int j=0; j<Degree +1; j++){
        std::cout<<coefficients[j]<<" x^ "<<j;
        if(j<Degree && coefficients[j+1]>=0){
            std::cout<<"+";
        }
    }
    std::cout<<"\t"<<std::endl;
}

//==================================================operator *========================================================//
template <int Degree>
template<int Degree2>
PolynomialItem <Degree + Degree2> PolynomialItem<Degree>::operator * (const PolynomialItem<Degree2> & p)const{
   PolynomialItem<Degree + Degree2> q;
    for(int i=0; i<Degree+1; i++){
        for(int j=0; j<Degree2+1; j++){
            q.coefficients[i+j] += coefficients[i] * p.coefficients[j];
        }
    }
    return q;
}

template <int Degree>
PolynomialItem <Degree>&PolynomialItem<Degree>::operator-=(const double s) {
    coefficients[0]-=s;
    return *this;
}

template <int Degree>
PolynomialItem<Degree> PolynomialItem<Degree>::operator-(const double s) const {
    PolynomialItem<Degree> q=*this;
    q.coefficients[0]-=s;
    return q;

}

template <int Degree>
PolynomialItem<Degree> &PolynomialItem<Degree>::operator+=(const double s) {
    coefficients[0]+=s;
    return *this;
}

template <int Degree>
PolynomialItem<Degree> PolynomialItem<Degree>::operator+(const double s) const {
    PolynomialItem<Degree> q=*this;
    q.coefficients[0]+=s;
    return q;
}

template <int Degree>
PolynomialItem<Degree> &PolynomialItem<Degree>::operator*=(const double s) {
    for(int i=0;i<Degree +1;i++){coefficients[i]*=s;}
    return *this;
}

template <int Degree>
PolynomialItem<Degree> PolynomialItem<Degree>::operator*(const double s) const {
    PolynomialItem<Degree> q;
    for(int i=0;i<Degree+1;i++){q.coefficients[i]=coefficients[i]*s;}
    return q;
}

template <int Degree>
PolynomialItem<Degree> &PolynomialItem<Degree>::operator/=(const double s) {
    for(int i=0;i<Degree+1;i++){coefficients[i]/=s;}
    return *this;
}

template <int Degree>
PolynomialItem<Degree> PolynomialItem<Degree>::operator/(const double s) const {
    PolynomialItem<Degree> q;
    for( int i=0 ; i<Degree+1 ; i++ ) q.coefficients[i] = coefficients[i]/s;
    return q;
}

template <int Degree>
PolynomialItem<Degree> PolynomialItem<Degree>::operator+( const PolynomialItem<Degree>& p ) const{

  PolynomialItem q;
  for(int i=0;i<Degree+1;i++){
      q.coefficients[i]=coefficients[i]+p.coefficients[i];
  }
  return q;
}

template <int Degree>
PolynomialItem<Degree> PolynomialItem<Degree>::operator-( const PolynomialItem<Degree>& p ) const{

    PolynomialItem q;
    for(int i=0;i<Degree+1;i++){
        q.coefficients[i]=coefficients[i]-p.coefficients[i];
    }
    return q;
}

template <int Degree>
PolynomialItem<Degree>& PolynomialItem<Degree>::operator +=(const PolynomialItem<Degree>& p){
    for(int i=0; i<Degree +1; i++){
        coefficients[i] += p.coefficients[i];
    }
    return *this;
}

template <int Degree>
PolynomialItem<Degree>& PolynomialItem<Degree>::operator -=(const PolynomialItem<Degree>& p){
    for(int i=0;i<Degree+1;i++){
        coefficients[i]-=p.coefficients[i];
    }
        return *this;
}

template <int Degree>
PolynomialItem<Degree> PolynomialItem<Degree>::operator-( void ) const{
    PolynomialItem q=*this;
    for(int i=0;i<Degree+1;i++){
        q.coefficients[i]=-q.coefficients[i];
    }
    return q;
}


//*********************************************CLASS STARTINGPOLYNORMIAL*********************************************//
//*******************************************************************************************************************//
//template<int Degree>
//StartingPolynomialItem<Degree>::StartingPolynomialItem(const StartingPolynomialItem<Degree> &sp) {
//    this->start = sp.start;
//    memcpy(p.coefficients, sp.p.coefficients, sizeof(double)*(Degree+1));
//
//}
//======================================================operator *===================================================//
template<int Degree>
template<int Degree2>
StartingPolynomialItem<Degree + Degree2>
        StartingPolynomialItem<Degree>::operator *(const StartingPolynomialItem<Degree2> &sp)const
{
    StartingPolynomialItem<Degree +Degree2> tsp;
    if (start > sp.start) {
        tsp.start = start;
    }
    else {
        tsp.start = sp.start;
    }
    tsp.p = this->p*sp.p;
    return tsp;
}

//=======================================================scale======================================================//
template<int Degree>
StartingPolynomialItem<Degree> StartingPolynomialItem<Degree>:: scale(const double &s) const{
    StartingPolynomialItem sp;
    sp.start= start*s;
    sp.p = p.scale(s);

    return sp;
}

//=======================================================shift=======================================================//
template<int Degree>
StartingPolynomialItem<Degree> StartingPolynomialItem<Degree>::shift(const double &t) const{
    StartingPolynomialItem sp;
    sp.start = start +t;
    sp.p = p.shift(t);
    return sp;
}

//=======================================================operator < =================================================//
template<int Degree>
int StartingPolynomialItem <Degree>::operator < (const StartingPolynomialItem <Degree>&sp) const{
    return start<sp.start? 1:0;
}

//=======================================================Compare=====================================================//
template<int Degree>
int StartingPolynomialItem <Degree>::Compare(const void *v1, const void *v2) {
    double d=((StartingPolynomialItem*)(v1))->start-((StartingPolynomialItem*)(v2))->start;
    if		(d<0)	{return -1;}
    else if	(d>0)	{return  1;}
    else			{return  0;}
}

