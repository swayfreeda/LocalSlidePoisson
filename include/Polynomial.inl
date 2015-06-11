//
// Created by sway on 5/30/15.
//

#include "Polynomial.h"
#include "string.h"


template<int Degree>
Polynomial<Degree>::Polynomial(void ) {
    polyCount = 0;
    polys = nullptr;
}


template<int Degree>
Polynomial<Degree>::Polynomial(const Polynomial<Degree> &p) {
    polyCount = 0;
    polys = nullptr;
    set(p.polyCount);
    memcpy(polys, p.polys, sizeof(StartingPolynomialItem<Degree>)*p.polyCount);

}

template<int Degree>
Polynomial<Degree> &Polynomial<Degree>::operator=(const Polynomial<Degree> &p) {
    set(p.polyCount);
    memcpy(polys,p.polys,sizeof(StartingPolynomialItem<Degree>)*p.polyCount);
    return *this;
}


template<int Degree>
Polynomial<Degree>::~Polynomial<Degree>(void) {
    if(polyCount){free(polys);}
    polyCount=0;
    polys=nullptr;
}


template<int Degree>
void Polynomial<Degree>::set(const int &size) {
    if(polyCount){free(polys);}
    polyCount=0;
    polys=nullptr;
    polyCount=size;
    if(size){
        polys=(StartingPolynomialItem<Degree>*)malloc(sizeof(StartingPolynomialItem<Degree>)*size);
        memset(polys,0,sizeof(StartingPolynomialItem<Degree>)*size);
    }
}

template<int Degree>
void Polynomial<Degree>::set(StartingPolynomialItem<Degree> *sps, const int &count) {
    int i,c=0;
    set(count);
    qsort(sps,count,sizeof(StartingPolynomialItem<Degree>),StartingPolynomialItem<Degree>::Compare);
    for( i=0 ; i<count ; i++ )
    {
        if( !c || sps[i].start!=polys[c-1].start ) polys[c++] = sps[i];
        else{polys[c-1].p+=sps[i].p;}
    }
    reset( c );
}

template<int Degree>
void Polynomial<Degree>::reset(const int &newSize) {
    polyCount=newSize;
    polys=(StartingPolynomialItem<Degree>*)realloc(polys,sizeof(StartingPolynomialItem<Degree>)*newSize);
}

template<int Degree>
Polynomial<Degree> Polynomial<Degree>:: ConstantFunction(const double& radius){

    if(Degree<0){
        std::cout<<"Could not set degree "<<Degree<<" polynomial as constant "<<std::endl;
        exit(0);
    }
    Polynomial q;
    q.set(2);

    q.polys[0].start=-radius;
    q.polys[1].start= radius;

    q.polys[0].p.coefficients[0]= 1.0;
    q.polys[1].p.coefficients[0]=-1.0;
    return q;

}

template<>
Polynomial<0> Polynomial<0>::GuassianApproximation(const double &width) {
    return ConstantFunction(width);
}

template<int Degree>
Polynomial <Degree>Polynomial <Degree>::GuassianApproximation(const double &width) {
     return Polynomial <Degree-1>::GuassianApproximation().MovingAverage(width);
}


template<int Degree>
Polynomial<Degree + 1> Polynomial<Degree>::MovingAverage(const double &radius) {

    Polynomial<Degree+1> A;
    PolynomialItem<Degree+1> p;
    StartingPolynomialItem<Degree+1>* sps;

    sps=(StartingPolynomialItem<Degree+1>*)malloc(sizeof(StartingPolynomialItem<Degree+1>)*polyCount*2);

    for(int i=0;i<int(polyCount);i++){
        sps[2*i  ].start=polys[i].start-radius;
        sps[2*i+1].start=polys[i].start+radius;
        p=polys[i].p.integral()-polys[i].p.integral()(polys[i].start);
        sps[2*i  ].p=p.shift(-radius);
        sps[2*i+1].p=p.shift( radius)*-1;
    }
    A.set(sps, polyCount*2);
    free(sps);
    return A*1.0/(2*radius);
}

template<int Degree>
Polynomial <Degree>&Polynomial<Degree>::operator+=(const double &s) {
    polys[0].p+=s;
    return *this;
}

template<int Degree>
Polynomial <Degree>&Polynomial<Degree>::operator-=(const double &s) {
    polys[0].p-=s;
    return *this;
}

template<int Degree>
Polynomial <Degree>&Polynomial<Degree>::operator*=(const double &s) {
    for(int i=0;i< polyCount;i++){polys[i].p*=s;}
    return *this;
}

template<int Degree>
Polynomial <Degree>&Polynomial<Degree>::operator/=(const double &s) {
    for(int i=0;i<polyCount;i++){polys[i].p/=s;}
    return *this;
}

template<int Degree>
Polynomial<Degree> Polynomial<Degree>::operator+(const double &s) const {
    Polynomial q=*this;
    q+=s;
    return q;
}

template<int Degree>
Polynomial<Degree> Polynomial<Degree>::operator-(const double &s) const {
    Polynomial q=*this;
    q-=s;
    return q;
}


template<int Degree>
Polynomial<Degree> Polynomial<Degree>::operator*(const double &s) const {
    Polynomial q=*this;
    q*=s;
    return q;
}


template<int Degree>
Polynomial<Degree> Polynomial<Degree>::operator/(const double &s) const {
    Polynomial q=*this;
    q/=s;
    return q;
}


template<int Degree>
Polynomial<Degree> Polynomial<Degree>::operator-(const Polynomial &p) const {
    Polynomial q;
    int i,j;
    size_t idx=0;
    q.set(polyCount+p.polyCount);
    i=j=-1;

    while(idx<q.polyCount){
        if		(j>=int(p.polyCount)-1)				{q.polys[idx]=  polys[++i];}
        else if	(i>=int(  polyCount)-1)				{q.polys[idx].start=p.polys[++j].start;q.polys[idx].p=p.polys[j].p*(-1.0);}
        else if(polys[i+1].start<p.polys[j+1].start){q.polys[idx]=  polys[++i];}
        else										{q.polys[idx].start=p.polys[++j].start;q.polys[idx].p=p.polys[j].p*(-1.0);}
        idx++;
    }
    return q;
}


template<int Degree>
Polynomial<Degree> Polynomial<Degree>::operator+(const Polynomial &p) const {
    Polynomial q;
    int i,j;
    size_t idx=0;
    q.set(polyCount+p.polyCount);
    i=j=-1;

    while(idx<q.polyCount){
        if		(j>= p.polyCount -1){
            q.polys[idx]=  polys[++i];
        }
        else if	(i>= polyCount -1){
            q.polys[idx]=p.polys[++j];
        }
        else if(polys[i+1].start<p.polys[j+1].start){
            q.polys[idx]=  polys[++i];
        }
        else{
            q.polys[idx]=p.polys[++j];
        }
        idx++;
    }
    return q;
}


template<int Degree>
Polynomial<Degree> Polynomial<Degree>::scale(const double &s) const {
    Polynomial q;
    q.set(polyCount);
    for(int i=0;i<polyCount;i++){
        q.polys[i]=polys[i].scale(s);
    }
    return q;
}


template<int Degree>
Polynomial<Degree> Polynomial<Degree>::shift(const double &t) const {

    Polynomial q;
    q.set(polyCount);
    for(size_t i=0;i<polyCount;i++){q.polys[i]=polys[i].shift(t);}
    return q;
}


template<int Degree>
Polynomial<Degree - 1> Polynomial<Degree>::derivative(void) const {
    Polynomial<Degree-1> q;
    q.set(polyCount);
    for(int i=0;i<polyCount;i++){
        q.polys[i].start=polys[i].start;
        q.polys[i].p=polys[i].p.derivative();
    }
    return q;
}


template<int Degree>
Polynomial<Degree + 1> Polynomial<Degree>::integral(void) const {
    int i;
    Polynomial<Degree+1> q;
    q.set(polyCount);
    for(i=0;i< polyCount;i++){
        q.polys[i].start=polys[i].start;
        q.polys[i].p=polys[i].p.integral();
        q.polys[i].p-=q.polys[i].p(q.polys[i].start);
    }
    return q;
}

template<int Degree>
void Polynomial<Degree>::printnl(void) const {

    PolynomialItem<Degree> p;

    if(!polyCount){
        std::cout<<"[-Infinity,Infinity]"<<std::endl;
    }
    else{
        for(size_t i=0;i<polyCount;i++){
            std::cout<<"[";
            if		(polys[i  ].start== DBL_MAX){ std::cout<<"Infinity,";}
            else if	(polys[i  ].start==-DBL_MAX){std::cout<<"-Infinity,";}
            else								{std::cout<<polys[i].start<<", ";}
            if(i+1==polyCount)					{std::cout<<"Infinity]";}
            else if (polys[i+1].start== DBL_MAX){std::cout<<"Infinity]";}
            else if	(polys[i+1].start==-DBL_MAX){std::cout<<"-Infinity]";}
            else								{std::cout<<polys[i+1].start<<"]";}
            p= p + (polys[i].p);
            std::cout<<"\t\t";
            p.printnl();
        }
    }
    std::cout<<std::endl;
}

template<int Degree>
double Polynomial<Degree>::integral(const double& tMin,const double& tMax) const{
    int m=1;
    double start,end,s,v=0;
    start=tMin;
    end=tMax;
    if(tMin>tMax){
        m=-1;
        start=tMax;
        end=tMin;
    }
    for(int i=0;i< polyCount && polys[i].start<end;i++){
        if(start<polys[i].start){s=polys[i].start;}
        else{s=start;}
        v+=polys[i].p.integral(s,end);
    }
    return v*m;

}

template<int Degree>
double Polynomial<Degree>::Integral(void ) const {
    return integral(polys[0].start,polys[polyCount-1].start);
}

template<int Degree>
double Polynomial<Degree>::operator()(const double& t) const {
    double v=0;
    for(int i=0;i< polyCount && t>polys[i].start;i++){
        v+=polys[i].p(t);
    }
    return v;
}


