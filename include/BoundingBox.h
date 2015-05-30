//
//  BoundingBox.h
//  Reconstructor
//
//  Created by Luwei Yang on 5/26/15.
//  Copyright (c) 2015 None. All rights reserved.
//

#ifndef __Reconstructor__BoundingBox__
#define __Reconstructor__BoundingBox__
#include "Vector3D.h"

using namespace DDG;

class BoundingBox {
    
public:
    BoundingBox(){};
    ~BoundingBox(){};
    Vector3D origin;
    Vector3D size;
    
};

#endif /* defined(__Reconstructor__BoundingBox__) */
