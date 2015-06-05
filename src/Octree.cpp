//
//  Octree.cpp
//  Reconstructor
//
//  Created by Luwei Yang on 5/26/15.
//  Copyright (c) 2015 None. All rights reserved.
//

#include "Octree.h"
#include "OctreeCell.h"
#include <float.h>
#include <queue>

using namespace DDG;

//============================================================ Octree =================================================//
// Octree constructor
Octree::Octree()
{
    // constructing root node
    root = OctreeCell::RootNode();

    //Initializing bounding Box
    boundingbox = new BoundingBox();
}

//============================================================ ~Octree ================================================//
// Destory the octree
Octree::~Octree()
{
    // Destory the octree cells
    if (this->root == NULL)
        this->deallocate();
    this->root = NULL;

    // Destory the bounding box
    delete this->boundingbox;
    this->boundingbox = NULL;
}

//============================================================rootNode===============================================//
// Return the ptr of root node.
OctreeCell* Octree::rootNode()
{
    return root;
}

//===========================================================getBoundingBox===========================================//
// Return the boundingbox
BoundingBox* Octree::getBoundingBox()
{
    return boundingbox;
}

//===========================================================Center===================================================//
// Return the center of the bounding box, note that the octree's
// center is (0.5, 0.5).
Vector3D Octree::center()
{
    return boundingbox->origin + boundingbox->size/2;
}

//==========================================================scale=====================================================//
double Octree::scale()
{
    // Determin the scale
    double maxWidth = - DBL_MAX;
    for (int i = 0; i < 3; i++) {
        if (boundingbox->size[i] > maxWidth) maxWidth = boundingbox->size[i];
    }

    return maxWidth/root->width();
}

//======================================================= generatOctreeFrom ===========================================//
// Generate the octree from a given mesh, the para depth is the max depth
// for the octree.
void Octree::generateOctreeFrom(DDG::Mesh mesh, int depth)
{
    maxDepth = depth;
    
    Vector3D max(-DBL_MAX, -DBL_MAX, -DBL_MAX);
    Vector3D min(DBL_MAX, DBL_MAX, DBL_MAX);
    
    // Iterate the vertices to get max and min
    for (VertexIter vertex = mesh.vertices.begin();
                    vertex != mesh.vertices.end();
                    vertex++)
    {

        // read the position of the point
        Vector3D *position = &(vertex->position);
        
        if(position->x < min.x) min.x = position->x;
        if(position->y < min.y) min.y = position->y;
        if(position->z < min.z) min.z = position->z;
        
        if(position->x > max.x) max.x = position->x;
        if(position->y > max.y) max.y = position->y;
        if(position->z > max.z) max.z = position->z;
    }
    
    Vector3D center = (max + min)/2;

    boundingbox->origin = min;
    boundingbox->size = max - min;

    // The maxwidth is the maximal of that of three dimensions of bounding box
    double maxWidth = -DBL_MAX;
    for (int i = 0; i < 3; i++) {
        double width = max[i] - min[i];
        if (width > maxWidth) {
            maxWidth = width;
        }
    }

    // Iterate the vertices' positon to generate the octree
    int nodeCount = 0;
    for (VertexIter vertex = mesh.vertices.begin();
                    vertex != mesh.vertices.end();
         vertex++) {
        OctreeCell *node = root;
        int currentDepth = 0;
        Vector3D position = vertex->position;

        // The center and width should refer to bounding box
        Vector3D currentCenter = center;
        double currentRadius = maxWidth / 2;

        // Search from root to create node
        while (currentDepth < maxDepth) {

            if (node->isLeaf()) {
                node->addChild();
                nodeCount += 8;
            }

            // Find the corner index of given parent node, note that position need to
            int childIndex = OctreeCell::CornerIndex(currentCenter, position);
            node = node->getChildren(childIndex);

            // Update the width
            currentRadius /= 2;

            // Update the center position
            ((childIndex & 1) != 0) ?
                    currentCenter.x += currentRadius : currentCenter.x -= currentRadius;
            ((childIndex & 2) != 0) ?
                    currentCenter.y += currentRadius : currentCenter.y -= currentRadius;
            ((childIndex & 4) != 0) ?
                    currentCenter.z += currentRadius : currentCenter.z -= currentRadius;

            // Update the depth, go deeper!
            currentDepth++;
        }

    }
}

void Octree::deallocate()
{
    std::queue<OctreeCell*> del_collections;

    // Find first leaf and push it to stack
    del_collections.push(root);

    OctreeCell* cell = NULL;

    // Iterate the cell from root to its childrens to destory octree
    while (!del_collections.empty())
    {
        cell = del_collections.front();
        del_collections.pop();

        // Push the children to stack
        if (!cell->isLeaf()) {
            for (int i = 0; i < 8; ++i) {
                OctreeCell *child = cell->getChildren(i);
                if (child != NULL)
                    del_collections.push(cell->getChildren(i));
            }
        }

        // Delete current octree cell
        delete cell;
        cell = NULL;
    }
}

// Count the cells, from root to its childrens
long long Octree::nodeCounts() {
    // If there is no nodes in octree
    if (root == NULL) return 0;

    std::queue<OctreeCell *> collections;

    // Find first leaf and push it to stack
    collections.push(root);

    OctreeCell *cell = NULL;
    long long count = 0;

    // Iterate the cells
    while (!collections.empty()) {
        cell = collections.front();
        collections.pop();

        // Push the children to stack
        if (!cell->isLeaf()) {
            for (int i = 0; i < 8; ++i) {
                OctreeCell *child = cell->getChildren(i);
                if (child != NULL)
                    collections.push(cell->getChildren(i));
            }
        }

        count++;
    }

    return count;
}

// Return the memroy load in bytes
size_t Octree::memoryUse() {
    int cellCounts = nodeCounts();

    size_t sizeOfCell = sizeof(OctreeCell);

    return sizeOfCell * cellCounts;
}
