#include "HalfEdge.h"
#include "Mesh.h"

namespace DDG
{
   double HalfEdge :: cotan( void ) const
   {
      if( onBoundary ) return 0.0;

      Vector3D p0 = next->next->vertex->position;
      Vector3D p1 = vertex->position;
      Vector3D p2 = next->vertex->position;

      Vector3D u = p1-p0;
      Vector3D v = p2-p0;

      return dot( u, v ) / cross( u, v ).norm();
   }

   Vector3D HalfEdge :: rotatedEdge( void ) const
   {
      if( onBoundary ) return Vector3D();
      
      Vector3D n = face->normal();
      Vector3D p0 = vertex->position;
      Vector3D p1 = flip->vertex->position;
      return cross( n, p1-p0 );
   }
}

