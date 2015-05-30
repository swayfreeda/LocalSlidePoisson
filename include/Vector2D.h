// -----------------------------------------------------------------------------
// libDDG -- Vector2D.h
// -----------------------------------------------------------------------------
//
// Vector2D represents an element of Euclidean 2-space, along with all the usual
// vectors space operations (addition, multiplication by scalars, etc.).  The
// inner product (i.e., scalar or dot product) is expressed using the global
// method dot():
//
//    Vector2D u, v;
//    double cosTheta = dot( u, v );
//
// Individual components can be accessed in two ways: either directly via the
// members x, y, and z:
//
//    Vector2D v;
//    cout << v.x << endl;
//    cout << v.y << endl;
//
// or by index:
//
//    Vector2D v;
//    for( int i = 0; i < 3; i++ )
//    {
//       cout << v[i] << endl;
//    }
//

#ifndef DDG_VECTOR_H
#define DDG_VECTOR_H

#include <iostream>

namespace DDG
{
    class Vector2D
    {
    public:
        Vector2D();
        // initializes all components to zero
        
        Vector2D( double x, double y);
        // initializes with specified components
        
        Vector2D( const Vector2D& v );
        // initializes from existing vector
        
        double& operator[] ( const int& index );
        // returns reference to the specified component (0-based indexing: x, y, z)
        
        const double& operator[] ( const int& index ) const;
        // returns const reference to the specified component (0-based indexing: x, y, z)
        
        Vector2D operator+( const Vector2D& v ) const;
        // addition
        
        Vector2D operator-( const Vector2D& v ) const;
        // subtraction
        
        Vector2D operator-( void ) const;
        // negation
        
        Vector2D operator*( const double& c ) const;
        // right scalar multiplication
        
        Vector2D operator/( const double& c ) const;
        // scalar division
        
        void operator+=( const Vector2D& v );
        // addition / assignment
        
        void operator-=( const Vector2D& v );
        // subtraction / assignment
        
        void operator*=( const double& c );
        // scalar multiplication / assignment
        
        void operator/=( const double& c );
        // scalar division / assignment
        
        double norm( void ) const;
        // returns Euclidean length
        
        double norm2( void ) const;
        // returns Euclidean length squared
        
        Vector2D unit( void ) const;
        // returns unit vector
        
        void normalize( void );
        // divides by Euclidean length
        
        Vector2D abs( void ) const;
        // returns vector containing magnitude of each component
        
        double x, y;
        // components
    };
    
    Vector2D operator* ( const double& c, const Vector2D& v );
    // left scalar multiplication
    
    double dot( const Vector2D& u, const Vector2D& v );
    // dot product (a.k.a. inner or scalar product)
    
    std::ostream& operator << (std::ostream& os, const Vector2D& o);
    // prints components
}

#endif

