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
#include <queue>

class OctreeCell;

class Octree {

    // Root node of octree
    OctreeCell *root_;
    
    // Bounding box
    BoundingBox *bounding_box_;
    
    // Max depth for octree
    int max_depth_;

public:
    
    // Octree constructor
    Octree();

    // Deallocate the octree
    ~Octree();

    // Insert a cell based on position
    void insert(Vector3D position);

    // Return the center of the bounding box, note that the octree's
    // center is (0.5, 0.5, 0.5).
    Vector3D boundingBoxCenter();

    // Return the scale of the octree, the scale is the ratio of minimal width
    // of Bounding Box and the width of octree's root node (default is 1.0).
    double boundingBoxScale();
    
    // Return the ptr of root node.
    OctreeCell* rootNode();
    
    // Generate the octree from a given mesh, the para depth is the max depth
    // for the octree.
    void generateOctreeFrom(DDG::Mesh *mesh, const int depth);

    // return the Bounding Box pointer of the
    BoundingBox* getBoundingBox();

    // Deallocate the octree, include the root node
    void deallocate();
    
    // Count the cells, from root to its childrens
    long long nodeCounts();
    
    // Return the memroy load in bytes
    size_t memoryUse();
    
    // Iterator class defination, BFS search the tree
    class CellIterator
    {
    public:
        // Default iterator with NULL ptr
        CellIterator();
        
        // Default iterator
        CellIterator(OctreeCell *cell);
        
        // Reload '++' operator for iterating next cell
        CellIterator& operator ++ (int);
        
        // Get the cell ptr
        inline OctreeCell* operator *()
        {
            return current_node;
        }
        
        // Treat the iterator as OctreeCell ptr
        inline OctreeCell* operator ->()
        {
            return current_node;
        }
        
        inline bool operator == (const CellIterator & rhs) const
        {
            return ((current_node != NULL)? this->current_node != rhs.current_node : rhs.current_node == NULL);
        }
        
        inline bool operator != (const CellIterator & rhs) const
        {
            // Currently we compare the ptr address
            return ((current_node != NULL)? this->current_node != rhs.current_node : rhs.current_node != NULL);
        }
        
    private:
        std::queue<OctreeCell*> iteration_queue;
        OctreeCell* current_node;
    };
    
    // Get the begin iterator
    CellIterator begin();
    
    // Get the end iterator
    static CellIterator end();
};

#endif /* defined(__Reconstructor__Octree__) */
