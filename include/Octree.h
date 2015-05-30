//
//  Octree.h
//  Reconstructor
//
//  Created by Luwei Yang on 5/26/15.
//  Copyright (c) 2015 None. All rights reserved.
//

#ifndef __Reconstructor__Octree__
#define __Reconstructor__Octree__

#include "Mesh.h"
#include "BoundingBox.h"
class OctreeCell;

class Octree {

    // Root node of octree
    OctreeCell *root;
    
    // Bounding box
    BoundingBox *boundingbox;
    
    // Max depth for octree
    int maxDepth;
    
public:
    
    // Octree constructor
    Octree();

    // Deallocate the octree
    ~Octree();
    
    // Return the center of the bounding box, note that the octree's
    // center is (0.5, 0.5, 0.5).
    Vector3D center();

    // Return the scale of the octree, the scale is the ratio of minimal width
    // of Bounding Box and the width of octree's root node (default is 1.0).
    double scale();
    
    // Return the ptr of root node.
    OctreeCell* rootNode();
    
    // Generate the octree from a given mesh, the para depth is the max depth
    // for the octree.
    void generateOctreeFrom(DDG::Mesh mesh, int depth);


    // return the Bounding Box pointer of the
    BoundingBox* getBoundingBox();


    // TODO
    int nodeCounts();
    
    // TODO
    int memoryUse();
};

#endif /* defined(__Reconstructor__Octree__) */
