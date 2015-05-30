#include <math.h>

#include "Vector2D.h"

namespace DDG
{
    Vector2D :: Vector2D( void )
    : x( 0. ),y( 0. )
    {}
    
    Vector2D :: Vector2D( double x0,
                          double y0
                         )
    : x( x0 ),y( y0 )
    {}
    
    Vector2D :: Vector2D( const Vector2D& v )
    : x( v.x ),y( v.y )
    {}
    
    double& Vector2D :: operator[]( const int& index )
    {
        return ( &x )[ index ];
    }
    
    const double& Vector2D :: operator[]( const int& index ) const
    {
        return ( &x )[ index ];
    }
    
    Vector2D Vector2D :: operator+( const Vector2D& v ) const
    {
        return Vector2D( x + v.x,
                        y + v.y);
    }
    
    Vector2D Vector2D :: operator-( const Vector2D& v ) const
    {
        return Vector2D( x - v.x,
                        y - v.y);
    }
    
    Vector2D Vector2D :: operator-( void ) const
    {
        return Vector2D( -x,
                        -y);
    }
    
    Vector2D Vector2D :: operator*( const double& c ) const
    {
        return Vector2D( x*c,
                        y*c);
    }
    
    Vector2D operator*( const double& c, const Vector2D& v )
    {
        return v*c;
    }
    
    Vector2D Vector2D :: operator/( const double& c ) const
    {
        return (*this) * ( 1./c );
    }
    
    void Vector2D :: operator+=( const Vector2D& v )
    {
        x += v.x;
        y += v.y;
    }
    
    void Vector2D :: operator-=( const Vector2D& v )
    {
        x -= v.x;
        y -= v.y;
    }
    
    void Vector2D :: operator*=( const double& c )
    {
        x *= c;
        y *= c;
    }
    
    void Vector2D :: operator/=( const double& c )
    {
        (*this) *= ( 1./c );
    }
    
    double Vector2D :: norm( void ) const
    {
        return sqrt( norm2());
    }
    
    double Vector2D :: norm2( void ) const
    {
        return dot( *this, *this );
    }
    
    void Vector2D :: normalize( void )
    {
        (*this) /= norm();
    }
    
    Vector2D Vector2D :: unit( void ) const
    {
        return (*this) / norm();
    }
    
    Vector2D Vector2D :: abs( void ) const
    {
        return Vector2D( fabs( x ),
                        fabs( y ));
    }
    
    double dot( const Vector2D& u, const Vector2D& v )
    {
        return u.x*v.x +
        u.y*v.y;
    }

    std::ostream& operator << (std::ostream& os, const Vector2D& o)
    {
        os << "[ "
        << o.x << " "
        << o.y << " "
        << " ]";
        
        return os;
    }
}
