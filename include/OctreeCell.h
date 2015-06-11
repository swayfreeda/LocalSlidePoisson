//
//  Octree.h
//  Reconstructor
//
//  Created by Luwei Yang on 5/26/15.
//  Copyright (c) 2015 None. All rights reserved.
//

#ifndef __Reconstructor__Datastructure__OctreeCell__
#define __Reconstructor__Datastructure__OctreeCell__

#include <iostream>
#include <assert.h>
#include "Vector3D.h"

#define DIM_X 0
#define DIM_Y 1
#define DIM_Z 2

using namespace DDG;

class OctreeCell {
protected:
    
    // The ptr point to the parent node
    OctreeCell *parent;
    
    // Ptrs point to childrens
    OctreeCell **childrens;

    // Ptrs to neighbors of the current cell
    OctreeCell **neighborCache;

    // The depth that the current cell located in an octree
    int depth;

    // Global offsets start from root node
    int globalOffsets[3];
    
    // Local offsets based on current depth
    int localOffsets[3];
    
    // Initiate the local offsets based on current depth.
    void initLocalOffsets();
    
    // Initial the global offsets based on root node. Note that this method
    // need parent's local offsets information
    void initGlobalOffsets();

    // Initialize the neighbors
    void initNeighbors();

public:
    
    // Constructor
    OctreeCell();
    
    ~OctreeCell();

    // Print info
    void printInfo();

    // Generate a root node
    static OctreeCell* RootNode();
    
    // Set the depth of cell
    inline void setDepth(int depth);
    
    // Return the depth of cell
    inline int getDepth();
    
    // Return the cell's width, assume that the root's width is 1.
    inline double width();
    
    // Return the cell's center, the range is [0, 1].
    Vector3D center();
    
    // Return if the current node is a leaf node.
    inline bool isLeaf();
    
    // Return is the current node is a root node.
    inline bool isRoot();
    
    // Return the ptr of child according to the given index.
    inline OctreeCell* getChildren(int index);
    
    // Return the ptr of parents.
    inline OctreeCell* getParent();
    
    
    
    // Find Corner position according to the corner index.
    Vector3D cornerPosition(int index);
    
    // Add Children to the current cell
    void addChild();
    
    // Return the cell's offset based on the current depth.
    // dimension: 0 for x, 1 for y, 2 for z;
    inline int localOffset(int dimension);
    
    // Return the cell's global offset based on the root node.
    // dimension: 0 for x, 1 for y, 2 for z;
    inline int globalOffset(int dimension);
    
    // Return the indices for corresponding dimension based on given node
    // respect to the parent node, not that para x,y,z are outputs.
    /* input : node
     * output : x, y, z*/
    inline static void DimensionIndices(OctreeCell *node, int &x, int &y, int &z);
    
    // Return the indices for corresponding dimension based on given corner_index
    inline static void DimensionIndices(int corner_index, int &x, int &y, int &z);

    // corner index corresponding to the index of it's children, the range of the index is [0,7]
    // Return the index of a corner that is close to a given position.
    inline static int CornerIndex(int x,int y,int z);

    // Return the index of a corner that is close to a given position.
    inline static int CornerIndex(Vector3D center, Vector3D position);
    
    // Return the index of a corner that is close to a given child node,
    // if child_node is root node, return -1.
    static int CornerIndex(OctreeCell *node);
    
    // Return the current cell's corner index based on its parent.
    int getCornerIndexOfParent();

    // Find the neighbor node at same depth via x,y,z, if has no neighbor node, return NULL
    OctreeCell *neighbor(int x, int y, int z);

    // Find the neighbor node via index, the range of index is [0, 27]
    OctreeCell *neighbor(int index);

    // Return the neibors array ptr
    OctreeCell **neighbors();

    // For DEBUG USE, TODO(Delete this later)
    bool DB_flag;
    
};

#include "OctreeCell.inl.h"

#endif /* defined(__Reconstructor__OctreeCell__) */
