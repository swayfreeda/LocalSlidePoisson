#include <math.h>

#include "Vector3D.h"

namespace DDG
{
   Vector3D :: Vector3D( void )
   : x( 0. ),
     y( 0. ),
     z( 0. )
   {}
   
   Vector3D :: Vector3D( double x0,
                     double y0,
                     double z0 )
   : x( x0 ),
     y( y0 ),
     z( z0 )
   {}
   
   Vector3D :: Vector3D( const Vector3D& v )
   : x( v.x ),
     y( v.y ),
     z( v.z )
   {}

   double& Vector3D :: operator[]( const int& index )
   {
      return ( &x )[ index ];
   }
   
   const double& Vector3D :: operator[]( const int& index ) const
   {
      return ( &x )[ index ];
   }
   
   Vector3D Vector3D :: operator+( const Vector3D& v ) const
   {
      return Vector3D( x + v.x,
                     y + v.y,
                     z + v.z );
   }
   
   Vector3D Vector3D :: operator-( const Vector3D& v ) const
   {
      return Vector3D( x - v.x,
                     y - v.y,
                     z - v.z );
   }
   
   Vector3D Vector3D :: operator-( void ) const
   {
      return Vector3D( -x,
                     -y,
                     -z );
   }
   
   Vector3D Vector3D :: operator*( const double& c ) const
   {
      return Vector3D( x*c,
                     y*c,
                     z*c );
   }
   
   Vector3D operator*( const double& c, const Vector3D& v )
   {
      return v*c;
   }
   
   Vector3D Vector3D :: operator/( const double& c ) const
   {
      return (*this) * ( 1./c );
   }
   
   void Vector3D :: operator+=( const Vector3D& v )
   {
      x += v.x;
      y += v.y;
      z += v.z;
   }
   
   void Vector3D :: operator-=( const Vector3D& v )
   {
      x -= v.x;
      y -= v.y;
      z -= v.z;
   }
   
   void Vector3D :: operator*=( const double& c )
   {
      x *= c;
      y *= c;
      z *= c;
   }
   
   void Vector3D :: operator/=( const double& c )
   {
      (*this) *= ( 1./c );
   }
   
   double Vector3D :: norm( void ) const
   {
      return sqrt( norm2());
   }
   
   double Vector3D :: norm2( void ) const
   {
      return dot( *this, *this );
   }
   
   void Vector3D :: normalize( void )
   {
      (*this) /= norm();
   }
   
   Vector3D Vector3D :: unit( void ) const
   {
      return (*this) / norm();
   }
   
   Vector3D Vector3D :: abs( void ) const
   {
      return Vector3D( fabs( x ),
                     fabs( y ),
                     fabs( z ) );
   }

   double dot( const Vector3D& u, const Vector3D& v )
   {
      return u.x*v.x +
             u.y*v.y +
             u.z*v.z ;
   }
   
   Vector3D cross( const Vector3D& u, const Vector3D& v )
   {
      return Vector3D( u.y*v.z - u.z*v.y,
                     u.z*v.x - u.x*v.z,
                     u.x*v.y - u.y*v.x );
   }
   
   std::ostream& operator << (std::ostream& os, const Vector3D& o)
   {
      os << "[ "
         << o.x << " "
         << o.y << " "
         << o.z
         << " ]";
   
      return os;
   }
}
