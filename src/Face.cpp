#include "Face.h"
#include "Mesh.h"
#include "Vector3D.h"

namespace DDG
{
   double Face::area( void ) const
   {
      Vector3D p0 = he->vertex->position;
      Vector3D p1 = he->next->vertex->position;
      Vector3D p2 = he->next->next->vertex->position;
      return cross( p1-p0, p2-p0 ).norm() / 2.;
   }

   Vector3D Face::normal( void ) const
   {
      Vector3D p0 = he->vertex->position;
      Vector3D p1 = he->next->vertex->position;
      Vector3D p2 = he->next->next->vertex->position;

      return cross( p1-p0, p2-p0 ).unit();
   }

   bool Face::isBoundary( void ) const
   {
      return he->onBoundary;
   }

   Vector3D Face :: circumcenter( void ) const
   {
      Vector3D p0 = he->vertex->position;
      Vector3D p1 = he->next->vertex->position;
      Vector3D n = he->rotatedEdge();
      double h  = 0.5*he->cotan();
      return 0.5*(p0+p1) + h*n;
   }
   
   Vector3D Face :: barycenter( void ) const
   {
      Vector3D p0 = he->vertex->position;
      Vector3D p1 = he->next->vertex->position;
      Vector3D p2 = he->next->next->vertex->position;
      return (p0 + p1 + p2)/3.;
   }
}

