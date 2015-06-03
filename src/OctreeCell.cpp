//
//  Octree.cpp
//  Reconstructor
//
//  Created by Luwei Yang on 5/26/15.
//  Copyright (c) 2015 None. All rights reserved.
//

#include "OctreeCell.h"

#define NEIBORINDEX(x, y, z) (x)+3*(y)+9*(z)
#define DIAGONALINDEX(index) (~index)  & 7

//===================================================Constructor======================================================//
// Constructor
OctreeCell::OctreeCell()
{
    this->parent = NULL;
    this->childrens = NULL;
    this->neighborCache = NULL;
    this->depth = 0;
    
    // Set the global and local offsets value to -1
    for (int i = 0; i < 3; i++) {
        this->globalOffsets[i] = -1;
        this->localOffsets[i] = -1;
    }
    
}

//==================================================Destructor========================================================//
// TODO[Luwei](Dellocate the Octree)
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

// Initialize the neighborCache, compute the neighbor cells across the face,edge,and corner
void OctreeCell::initNeighbors() {

    if (neighborCache != NULL) { delete neighborCache; };

    // Initialize the neighborCache ptr, TODO[Luwei](better initialization method for ptr array)
    neighborCache = new OctreeCell *[27];
    for (int i = 0; i < 27; ++i) {
        neighborCache[i] = NULL;
    }

    neighborCache[NEIBORINDEX(1, 1, 1)] = this;

    if (parent == NULL) return;

    OctreeCell *parent = this->parent;
    int childIndex = this->getCornerIndexOfParent();

    // Get dimension offset (x,y,z) for the current node and the diagonal node
    int x1, y1, z1, x2, y2, z2;
    DimensionIndices(childIndex, x1, y1, z1);
    DimensionIndices(DIAGONALINDEX(childIndex), x2, y2, z2);

    // Add the siblings to the neighborCache
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                int index = CornerIndex(i, j, k);
                neighborCache[NEIBORINDEX(x2 + i, y2 + j, z2 + k)] = parent->childrens[index];
            }
        }
    }

    // Get the parents neighborCache
    OctreeCell **parentNeighbors = NULL;
    if (parent->neighborCache == NULL)   //if parent has no neighborCache cache, initialize the neighborCache
        parent->initNeighbors();
    parentNeighbors = parent->neighborCache;

    // Set the neighborCache across the face
    int i, j, k;
    i = x1 << 1;
    OctreeCell *adjFaceNode1 = parentNeighbors[NEIBORINDEX(i, 1, 1)];
    if (adjFaceNode1 != NULL) {
        if (adjFaceNode1->childrens == NULL) { adjFaceNode1->addChild(); }
        for (j = 0; j < 2; j++) {
            for (k = 0; k < 2; k++) {
                neighborCache[NEIBORINDEX(i, y2 + j, z2 + k)] =
                        adjFaceNode1->childrens[CornerIndex(x2, j, k)];
            }
        }
    }

    j = y1 << 1;
    OctreeCell *adjFaceNode2 = parentNeighbors[NEIBORINDEX(1, j, 1)];
    if (adjFaceNode2 != NULL) {
        if (adjFaceNode2->childrens == NULL) { adjFaceNode2->addChild(); }
        for (i = 0; i < 2; i++) {
            for (k = 0; k < 2; k++) {
                neighborCache[NEIBORINDEX(x2 + i, j, z2 + k)] =
                        adjFaceNode2->childrens[CornerIndex(i, y2, k)];
            }
        }
    }

    k = z1 << 1;
    OctreeCell *adjFaceNode3 = parentNeighbors[NEIBORINDEX(1, 1, k)];
    if (adjFaceNode3 != NULL) {
        if (adjFaceNode3->childrens == NULL) { adjFaceNode3->addChild(); }
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 2; j++) {
                neighborCache[NEIBORINDEX(x2 + i, y2 + j, k)] =
                        adjFaceNode3->childrens[CornerIndex(i, j, z2)];
            }
        }
    }

    // Set the neighborCache across the edge
    i = x1 << 1;
    j = y1 << 1;
    OctreeCell *adjEdgeNode1 = parentNeighbors[NEIBORINDEX(i, j, 1)];
    if (adjEdgeNode1 != NULL) {
        if (adjEdgeNode1->childrens == NULL) { adjEdgeNode1->addChild(); }
        for (k = 0; k < 2; k++) {
            neighborCache[NEIBORINDEX(i, j, z2 + k)] =
                    adjEdgeNode1->childrens[CornerIndex(x2, y2, k)];
        }
    }

    i = x1 << 1;
    k = z1 << 1;
    OctreeCell *adjEdgeNode2 = parentNeighbors[NEIBORINDEX(i, 1, k)];
    if (adjEdgeNode2 != NULL) {
        if (adjEdgeNode2->childrens == NULL) { adjEdgeNode2->addChild(); }
        for (j = 0; j < 2; j++) {
            neighborCache[NEIBORINDEX(i, y2 + j, k)] =
                    adjEdgeNode2->childrens[CornerIndex(x2, j, z2)];
        }
    }

    j = y1 << 1;
    k = z1 << 1;
    OctreeCell *adjEdgeNode3 = parentNeighbors[NEIBORINDEX(1, j, k)];
    if (adjEdgeNode3 != NULL) {
        if (adjEdgeNode3->childrens == NULL) { adjEdgeNode3->addChild(); }
        for (i = 0; i < 2; i++) {
            neighborCache[NEIBORINDEX(x2 + i, j, k)] =
                    adjEdgeNode3->childrens[CornerIndex(i, y2, z2)];
        }
    }

    // Set neighborCache across the corner
    i = x1 << 1;
    j = y1 << 1;
    k = z1 << 1;
    OctreeCell *adjCornerNode = parentNeighbors[NEIBORINDEX(i, j, k)];
    if (adjCornerNode != NULL) {
        if (adjCornerNode->childrens == NULL) { adjCornerNode->addChild(); }
        neighborCache[NEIBORINDEX(i, j, k)] = adjCornerNode->childrens[CornerIndex(x2, y2, z2)];
    }

}

// Return the neighbor cell based on offsets (x,y,z)
OctreeCell *OctreeCell::neighbor(int x, int y, int z) {
    if (neighborCache == NULL) initNeighbors();
    return neighborCache[NEIBORINDEX(x, y, z)];
}

// Return the neighbor cell based on neighbor index [0, 27]
OctreeCell *OctreeCell::neighbor(int index) {
    if (neighborCache == NULL)
        initNeighbors();
    return neighborCache[index];
}

// Return the neighbors array ptr
OctreeCell **OctreeCell::neighbors() {
    if (neighborCache == NULL)
        initNeighbors();
    return neighborCache;
}
