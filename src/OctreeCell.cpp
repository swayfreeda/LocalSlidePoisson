//
//  Octree.cpp
//  Reconstructor
//
//  Created by Luwei Yang on 5/26/15.
//  Copyright (c) 2015 None. All rights reserved.
//

#include "OctreeCell.h"
#include "string.h"

//===================================================Constructor======================================================//
// Constructor
OctreeCell::OctreeCell()
{
    this->parent = NULL;
    this->childrens = NULL;
    this->neibors = NULL;
    this->depth = 0;
    
    // Set the global and local offsets value to -1
    for (int i = 0; i < 3; i++) {
        this->globalOffsets[i] = -1;
        this->localOffsets[i] = -1;
    }
    
}


//==================================================Destructor========================================================//
// TODO Dellocate the Octree
OctreeCell::~OctreeCell()
{
}

//==================================================RootNode==========================================================//
// Generate a root node
OctreeCell* OctreeCell::RootNode()
{
    OctreeCell *rootCell = new OctreeCell();
    rootCell->setDepth(0);
    
    // Set the local and global offsets to 0
    for (int i = 0; i < 3; i++) {
        rootCell->globalOffsets[i] = 0;
        rootCell->localOffsets[i] = 0;
    }
    
    return rootCell;
}
//====================================================center==========================================================//
// Return the cell's center, the range is [0, 1]
Vector3D OctreeCell::center()
{
    Vector3D ceter;
    double currentWidth = this->width();
    
    // Check if have assigned the local offsets
    if (localOffsets[0] == -1) initLocalOffsets();
        
    ceter.x = (0.5 + localOffsets[0]) * currentWidth;
    ceter.y = (0.5 + localOffsets[1]) * currentWidth;
    ceter.z = (0.5 + localOffsets[2]) * currentWidth;
    
    return ceter;
}

//===================================================initGlobalOffset==============================================//
//offset of all the depths
// Initial the global offsets based on root node. Note that this method
// need parent's local offsets information
void OctreeCell::initGlobalOffsets()
{
    if (this->parent == NULL) {
        memset(globalOffsets, 0, sizeof(globalOffsets));
        return;
    }
    // index of the current cell according to it's parent
    int cornerIndex = getCornerIndexOfParent();
    
    // Get indices for corresponding dimension according to
    int x, y ,z;
    DimensionIndices(cornerIndex, x, y, z);

    // temporary variables
    int tempOffset[3];

    // Get local offsets based on parent's local offsets
    tempOffset[0] = (parent->localOffsets[0] << 1) + x;
    tempOffset[1] = (parent->localOffsets[1] << 1) + y;
    tempOffset[2] = (parent->localOffsets[2] << 1) + z;
    
    // Transform to Gobal Offset
    tempOffset[0] = (int)((1 << depth) + tempOffset[0] - 1);
    tempOffset[1] = (int)((1 << depth) + tempOffset[1] - 1);
    tempOffset[2] = (int)((1 << depth) + tempOffset[2] - 1);
    
    globalOffsets[0] = tempOffset[0];
    globalOffsets[1] = tempOffset[1];
    globalOffsets[2] = tempOffset[2];
}
//=================================================initLocalOffsets==================================================//
// offset on the same depth
// Initiate the local offsets based on current depth,
void OctreeCell::initLocalOffsets()
{
    if (globalOffsets[0] == -1)
        initGlobalOffsets();
    
    int currentDepth = this->depth;
    localOffsets[0] = ((int)(globalOffsets[0]) + 1) & (~(1 << currentDepth));
    localOffsets[1] = ((int)(globalOffsets[1]) + 1) & (~(1 << currentDepth));
    localOffsets[2] = ((int)(globalOffsets[2]) + 1) & (~(1 << currentDepth));
}

//============================================cornerPosition========================================================//
// Find Corner position of a children according to it's index.
Vector3D OctreeCell::cornerPosition(int index)
{
    Vector3D currentCenter = this->center();
    double width = this->width()/2;
    
    double x = 0,y = 0,z = 0;
    
    (index & 1) == 1? x = currentCenter.x + width:x = currentCenter.x - width;
    (index & 2) == 2? y = currentCenter.y + width:y = currentCenter.y - width;
    (index & 4) == 4? z = currentCenter.z + width:z = currentCenter.z - width;
    
    return Vector3D(x, y, z);
}

//============================================addChild===============================================================//
// Add Children to the current cell
void OctreeCell::addChild()
{
    childrens = new OctreeCell*[8];
    for (int i = 0; i < 8; i ++) {
        childrens [i] = NULL;
    }

    for (int i = 0; i < 8; i++) {
        childrens [i] = new OctreeCell();
        // add parent
        childrens [i]->parent = this;
        // add depth
        childrens [i]->depth = depth + 1;
    }

    // set the global and local offsets of children
    for(int i=0; i< 8; i++){
        childrens[i]->initGlobalOffsets();
        childrens[i]->initLocalOffsets();
    }
}

//================================================CornerIndex=========================================================//
// Return the index of a node according to it's parent, maybe the function name should be changed to avoid ambiguity
// Return the index of a corner that is close to a given child node,
// if child_node is root node, return -1.
int OctreeCell::CornerIndex(OctreeCell *child_node)
{
    // If node is parent
    if (child_node->parent == NULL)
        return 0;
    
    OctreeCell *parent = child_node->parent;
    
    // If the child_node is root node, return -1
    if (parent == NULL)
        return -1;
    
    for (int i = 0; i < 8; i++)    {
        OctreeCell *cell = parent->childrens[i];
        if (cell == child_node)
            return i;
    }
    
    return -1;
}

//===========================================getCornerIndexOfParent===================================================//
// Return the current cell's corner index based on its parent
int OctreeCell::getCornerIndexOfParent()
{
    return CornerIndex(this);
}

