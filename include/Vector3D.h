// -----------------------------------------------------------------------------
// libDDG -- Vector3D.h
// -----------------------------------------------------------------------------
//
// Vector3D represents an element of Euclidean 3-space, along with all the usual
// vectors space operations (addition, multiplication by scalars, etc.).  The
// inner product (i.e., scalar or dot product) is expressed using the global
// method dot():
//
//    Vector3D u, v;
//    double cosTheta = dot( u, v );
//
// and the cross product is expressed using the global method cross():
//
//    Vector3D u, v, w;
//    w = cross( u, v );
//
// Individual components can be accessed in two ways: either directly via the
// members x, y, and z:
//
//    Vector3D v;
//    cout << v.x << endl;
//    cout << v.y << endl;
//    cout << v.z << endl;
//
// or by index:
//
//    Vector3D v;
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
   class Vector3D
   {
      public:
         Vector3D();
         // initializes all components to zero

         Vector3D( double x, double y, double z);
         // initializes with specified components

         Vector3D( const Vector3D& v );
         // initializes from existing vector

         double& operator[] ( const int& index );
         // returns reference to the specified component (0-based indexing: x, y, z)

         const double& operator[] ( const int& index ) const;
         // returns const reference to the specified component (0-based indexing: x, y, z)

         Vector3D operator+( const Vector3D& v ) const;
         // addition

         Vector3D operator-( const Vector3D& v ) const;
         // subtraction

         Vector3D operator-( void ) const;
         // negation

         Vector3D operator*( const double& c ) const;
         // right scalar multiplication

         Vector3D operator/( const double& c ) const;
         // scalar division

         void operator+=( const Vector3D& v );
         // addition / assignment

         void operator-=( const Vector3D& v );
         // subtraction / assignment

         void operator*=( const double& c );
         // scalar multiplication / assignment

         void operator/=( const double& c );
         // scalar division / assignment

         double norm( void ) const;
         // returns Euclidean length

         double norm2( void ) const;
         // returns Euclidean length squared

         Vector3D unit( void ) const;
         // returns unit vector

         void normalize( void );
         // divides by Euclidean length

         Vector3D abs( void ) const;
         // returns vector containing magnitude of each component
   
         double x, y, z;
         // components
   };
   
   Vector3D operator* ( const double& c, const Vector3D& v );
   // left scalar multiplication

   double dot( const Vector3D& u, const Vector3D& v );
   // dot product (a.k.a. inner or scalar product)

   Vector3D cross( const Vector3D& u, const Vector3D& v );
   // cross product

   std::ostream& operator << (std::ostream& os, const Vector3D& o);
   // prints components
}

#endif

