//
// Created by Luwei Yang on 6/9/15.
//

#ifndef RECONSTRUCTOR_MESHDATA_H
#define RECONSTRUCTOR_MESHDATA_H

#include <vector>
#include "Vector3D.h"
#include "Index.h"

namespace DDG
{
    class MeshData
    {
    public:
        std::vector<Vector3D> positions;
        std::vector<Vector3D> texcoords;
        std::vector<Vector3D> normals;
        std::vector< std::vector< Index > > indices;
    };

}

#endif //RECONSTRUCTOR_MESHDATA_H
