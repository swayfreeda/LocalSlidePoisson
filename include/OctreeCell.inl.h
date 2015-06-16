//
//  Octree.cpp
//  Reconstructor
//
//  Created by Luwei Yang on 5/26/15.
//  Copyright (c) 2015 None. All rights reserved.
//

#include "OctreeCell.h"

//======================================================getChildren==================================================//
OctreeCell* OctreeCell::getChildren(int index)
{
    if(index > 7)
        assert("Out of range of indices.");

    if (isLeaf())
        return NULL;

    return childrens_[index];
}

//======================================================getParent====================================================//
OctreeCell* OctreeCell::getParent()
{
    return parent_;
}

//=======================================================setDepth====================================================//
void OctreeCell::setDepth(int depth)
{
    this->depth_ = depth;
}

int OctreeCell::getDepth()
{
    return this->depth_;
}

//=======================================================width=======================================================//
// Return the cell's width, assume that the root's width is 1.
double OctreeCell::width()
{
    return (1.0 / (double)(1 << depth_));
}

//=======================================================localOffset==================================================//
// return the local offset of a dimension (x, y, or z)
// Return the cell's offset based on the current depth.
// dimension: 0 for x, 1 for y, 2 for z;
int OctreeCell::localOffset(int dimension)
{
    if(dimension > 3)
        assert("The dimension is less than 3.");
    return local_offsets_[dimension];
}

//======================================================globalOffset==================================================//
// Return the cell's global offset based on the root node.
// dimension: 0 for x, 1 for y, 2 for z;
int OctreeCell::globalOffset(int dimension)
{
    if(dimension > 3)
        assert("The dimension is less than 3.");

    return global_offsets_[dimension];
}

//=====================================================isLeaf=========================================================//
// A node is a leaf node if it has no children
// Return if the current node is a leaf node.
bool OctreeCell::isLeaf()
{
    return (this->childrens_ == NULL);
}

//=====================================================isRoot=========================================================//
// A cell is a root node is it has no parent
// Return is the current node is a root node.
bool OctreeCell::isRoot()
{
    return (this->parent_ == NULL);
}

//=====================================================DimensionIndices===============================================//
// Return the indices for corresponding dimension based on given node
// respect to the parent node, note that para x,y,z are outputs.
void OctreeCell::DimensionIndices(OctreeCell *node, int &x, int &y, int &z)
{
    int index = CornerIndex(node);
    assert(index<8);
    DimensionIndices(index, x, y, z);
    
    return;
}

//====================================================DinmensionIndices===============================================//
// Return the indices for corresponding dimension based on given corner_index
void OctreeCell::DimensionIndices(int corner_index, int &x, int &y, int &z)
{
    if(corner_index < 8)
        assert("The corner index should less than 8.");

    ((corner_index & 1) == 1)? x = 1:x = 0;
    ((corner_index & 2) == 2)? y = 1:y = 0;
    ((corner_index & 4) == 4)? z = 1:z = 0;
    
    return;
}

//===================================================CornerIndex======================================================//
// Given the center of a node and a spatial position of a point, return the index of the children that is closest to
// the center
// Return the index of a corner based on (x,y,z).
// Indices:
//   btm layer    top layer
//   | 2 | 3 |    | 6 | 7 |
//   | 0 | 1 |    | 4 | 5 |
int OctreeCell::CornerIndex(Vector3D center, Vector3D position)
{
    int index = 0;
    
    // Mark the 1st,2nd,3rd bit, for the x,y,z dimension respectively
    if (position.x > center.x) { index |= 1; }
    if (position.y > center.y) { index |= 2; }
    if (position.z > center.z) { index |= 4; }
    
    return index;
}
//=================================================COrnerIndex========================================================//
// 2^2 * z + 2 * y + x
// Return the index of a corner that is close to a given position.
int OctreeCell::CornerIndex(int x, int y,int z)
{
    return (z << 2) | (y << 1) | x;
}


