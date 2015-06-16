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

using namespace DDG;

//============================================================ Octree =================================================//
// Octree constructor
Octree::Octree()
{
    // constructing root node
    root_ = OctreeCell::RootNode();
    
    //Initializing bounding Box
    bounding_box_ = new BoundingBox();
}

//============================================================ ~Octree ================================================//
// Destory the octree
Octree::~Octree()
{
    // Destory the octree cells
    if (this->root_ == NULL)
        this->deallocate();
    this->root_ = NULL;
    
    // Destory the bounding box
    delete this->bounding_box_;
    this->bounding_box_ = NULL;
}

//============================================================rootNode===============================================//
// Return the ptr of root node.
OctreeCell* Octree::rootNode()
{
    return root_;
}

//===========================================================getBoundingBox===========================================//
// Return the bounding_box_
BoundingBox* Octree::getBoundingBox()
{
    return bounding_box_;
}

//===========================================================Center===================================================//
// Return the boundingBoxCenter of the bounding box, note that the octree's
// boundingBoxCenter is (0.5, 0.5).
Vector3D Octree::boundingBoxCenter()
{
    return bounding_box_->origin + bounding_box_->size/2;
}

//==========================================================boundingBoxScale=====================================================//
double Octree::boundingBoxScale()
{
    // Determin the boundingBoxScale
    double max_width = - DBL_MAX;
    for (int i = 0; i < 3; i++) {
        if (bounding_box_->size[i] > max_width) max_width = bounding_box_->size[i];
    }
    
    return max_width / root_->width();
}

//======================================================= generatOctreeFrom ===========================================//
// Generate the octree from a given mesh, the para depth is the max depth
// for the octree.
void Octree::generateOctreeFrom(DDG::Mesh *mesh, const int depth)
{
    max_depth_ = depth;
    
    Vector3D max(-DBL_MAX, -DBL_MAX, -DBL_MAX);
    Vector3D min(DBL_MAX, DBL_MAX, DBL_MAX);
    
    // Iterate the vertices to get max and min
    for (VertexIter vertex = mesh->vertices.begin();
         vertex != mesh->vertices.end();
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
    
    bounding_box_->origin = min;
    bounding_box_->size = max - min;
    
    // The maxwidth is the maximal of that of three dimensions of bounding box
    double max_width = -DBL_MAX;
    for (int i = 0; i < 3; i++) {
        double width = max[i] - min[i];
        if (width > max_width) {
            max_width = width;
        }
    }
    
    // Iterate the vertices' positon to generate the octree
    int node_count = 0;
    for (VertexIter vertex = mesh->vertices.begin();
         vertex != mesh->vertices.end();
         vertex++) {

        OctreeCell *node = root_;
        int current_depth = 0;

        // poisition of the vertex
        Vector3D position = vertex->position;
        
        // The boundingBoxCenter and width should refer to bounding box
        Vector3D current_center = center;
        double current_radius = max_width / 2;
        
        // Search from root to create node
        while (current_depth < max_depth_) {
            
            if (node->isLeaf()) {
                node->addChild();
                node_count += 8;
            }
            
            // Find the corner index of given parent node, note that position need to
            // find which node cell the point falls in
            int child_index = OctreeCell::CornerIndex(current_center, position);
            node = node->childrens(child_index);
            
            // Update the width
            current_radius /= 2;
            
            // Update the boundingBoxCenter position
            ((child_index & 1) != 0) ?
            current_center.x += current_radius : current_center.x -= current_radius;
            ((child_index & 2) != 0) ?
            current_center.y += current_radius : current_center.y -= current_radius;
            ((child_index & 4) != 0) ?
            current_center.z += current_radius : current_center.z -= current_radius;
            
            // Update the depth, go deeper!
            current_depth++;
        }
        
    }
    
}

void Octree::deallocate()
{
    std::queue<OctreeCell*> del_collections;
    
    // Find first leaf and push it to stack
    del_collections.push(root_);
    
    OctreeCell* cell = NULL;
    
    // Iterate the cell from root to its childrens to destory octree
    while (!del_collections.empty())
    {
        cell = del_collections.front();
        del_collections.pop();
        
        // Push the children to stack
        if (!cell->isLeaf()) {
            for (int i = 0; i < 8; ++i) {
                OctreeCell *child = cell->childrens(i);
                if (child != NULL)
                    del_collections.push(cell->childrens(i));
            }
        }
        
        // Delete current octree cell
        delete cell;
        cell = NULL;
    }
    
    this->root_ = NULL;
}

// Count the cells, from root to its childrens
long long Octree::nodeCounts() {
    // If there is no nodes in octree
    if (root_ == NULL) return 0;
    
    std::queue<OctreeCell *> collections;
    
    // Find first leaf and push it to stack
    collections.push(root_);
    
    OctreeCell *cell = NULL;
    long long count = 1;
    
    // Iterate the cells
    while (!collections.empty()) {
        cell = collections.front();
        collections.pop();
        
        // Push the children to stack
        if (!cell->isLeaf()) {
            for (int i = 0; i < 8; ++i) {
                OctreeCell *child = cell->childrens(i);
                if (child != NULL)
                    collections.push(cell->childrens(i));
            }
        }
        
        count++;
    }
    
    return count;
}

// Return the memroy load in bytes
size_t Octree::memoryUse() {
    long long cell_counts = nodeCounts();
    
    size_t size_of_Cell = sizeof(OctreeCell);
    
    return size_of_Cell * cell_counts;
}

// Get the begin iterator
Octree::CellIterator Octree::begin()
{
    return Octree::CellIterator(this->root_);
}

// Get the end iterator
Octree::CellIterator Octree::end()
{
    return Octree::CellIterator(NULL);
}

// Default constructor of iterator, init with NULL ptr
Octree::CellIterator::CellIterator()
: current_node(NULL)
{
}

// Constructor of iterator with exist cell as beginer
Octree::CellIterator::CellIterator(OctreeCell *cell)
: current_node(cell)
{
    if(cell != NULL)
    {
        // Enqueue the cells children
        if (!current_node->isLeaf()) {
            for (int i = 0; i < 8; i ++) {
                iteration_queue.push(current_node->childrens(i));
            }
        }
    }
}

// Iterate next cell
Octree::CellIterator& Octree::CellIterator::operator++(int)
{
    // Check if have iterated to the end
    if (iteration_queue.empty())
    {
        current_node = NULL;
        return (*this);
    }
    
    current_node = iteration_queue.front();
    iteration_queue.pop();
    
    // Enqueue the children
    if (!current_node->isLeaf()) {
        for (int i = 0; i < 8; i ++) {
            iteration_queue.push(current_node->childrens(i));
        }
    }
    
    return (*this);
}

// Insert a cell based on position
// TODO[Luwei]: implement this method later
void Octree::insert(Vector3D position) {

}
