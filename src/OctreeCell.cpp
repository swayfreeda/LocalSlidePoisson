//
//  Octree.cpp
//  Reconstructor
//
//  Created by Luwei Yang on 5/26/15.
//  Copyright (c) 2015 None. All rights reserved.
//

#include "OctreeCell.h"
#include <string.h>

#define NEIBORINDEX(x, y, z) ((x)+3*(y)+9*(z))
#define DIAGONALINDEX(index) ((~index) & 7)

//===================================================Constructor======================================================//
// Constructor
OctreeCell::OctreeCell():
    parent_(NULL),
    childrens_(NULL),
    neighbor_cache_(NULL),
    depth_(0)
{

    // Set the global and local offsets value to -1
    for (int i = 0; i < 3; i++) {
        this->global_offsets_[i] = -1;
        this->local_offsets_[i] = -1;
    }
    
}

//==================================================Destructor========================================================//
// TODO[Luwei]: Deallocate the cell
// Destructor of octree cell will not destory the childrens
OctreeCell::~OctreeCell()
{
    for (int k = 0; k < 3; ++k) {
        local_offsets_[k] = 0;
        global_offsets_[k] = 0;
    }

    if(this->childrens_ != NULL){
        for (int i = 0; i < 8; ++i) {
            this->childrens_[i] = NULL;
        }
        delete this->childrens_;
        this->childrens_ = NULL;
    }

    this->parent_ = NULL;

    if(this->neighbor_cache_ != NULL)
    {
        for (int j = 0; j < 27; ++j) {
            this->neighbor_cache_[j] = NULL;
        }
        delete this->neighbor_cache_;
        this->neighbor_cache_ = NULL;
    }

    this->depth_ = 0;

}

//==================================================RootNode==========================================================//
// Generate a root node
OctreeCell* OctreeCell::RootNode()
{
    OctreeCell *rootCell = new OctreeCell();
    rootCell->setDepth(0);
    
    // Set the local and global offsets to 0
    for (int i = 0; i < 3; i++) {
        rootCell->global_offsets_[i] = 0;
        rootCell->local_offsets_[i] = 0;
    }
    
    return rootCell;
}
//====================================================boundingBoxCenter==========================================================//
// Return the cell's boundingBoxCenter, the range is [0, 1]
Vector3D OctreeCell::center()
{
    Vector3D ceter;
    double current_width = this->width();
    
    // Check if have assigned the local offsets
    if (local_offsets_[0] == -1) initLocalOffsets();
        
    ceter.x = (0.5 + local_offsets_[0]) * current_width;
    ceter.y = (0.5 + local_offsets_[1]) * current_width;
    ceter.z = (0.5 + local_offsets_[2]) * current_width;
    
    return ceter;
}

//===================================================initGlobalOffset==============================================//
//offset of all the depths
// Initial the global offsets based on root node. Note that this method
// need parent's local offsets information
void OctreeCell::initGlobalOffsets()
{
    if (this->parent_ == NULL) {
        memset(global_offsets_, 0, sizeof(global_offsets_));
        return;
    }
    // index of the current cell according to it's parent
    int corner_index = getCornerIndexOfParent();
    
    // Get indices for corresponding dimension according to
    int x, y ,z;
    DimensionIndices(corner_index, x, y, z);

    // temporary variables
    int temp_offset[3];

    // Get local offsets based on parent's local offsets
    temp_offset[0] = (parent_->local_offsets_[0] << 1) + x;
    temp_offset[1] = (parent_->local_offsets_[1] << 1) + y;
    temp_offset[2] = (parent_->local_offsets_[2] << 1) + z;
    
    // Transform to Gobal Offset
    temp_offset[0] = (int)((1 << depth_) + temp_offset[0] - 1);
    temp_offset[1] = (int)((1 << depth_) + temp_offset[1] - 1);
    temp_offset[2] = (int)((1 << depth_) + temp_offset[2] - 1);
    
    global_offsets_[0] = temp_offset[0];
    global_offsets_[1] = temp_offset[1];
    global_offsets_[2] = temp_offset[2];
}
//=================================================initLocalOffsets==================================================//
// offset on the same depth
// Initiate the local offsets based on current depth,
void OctreeCell::initLocalOffsets()
{
    if (global_offsets_[0] == -1)
        initGlobalOffsets();
    
    int current_depth = this->depth_;
    local_offsets_[0] = ((int)(global_offsets_[0]) + 1) & (~(1 << current_depth));
    local_offsets_[1] = ((int)(global_offsets_[1]) + 1) & (~(1 << current_depth));
    local_offsets_[2] = ((int)(global_offsets_[2]) + 1) & (~(1 << current_depth));
}

//============================================cornerPosition========================================================//
// Find Corner position of a children according to it's index.
Vector3D OctreeCell::cornerPosition(int index)
{
    Vector3D current_center = this->center();
    double width = this->width()/2;
    
    double x = 0,y = 0,z = 0;
    
    (index & 1) == 1? x = current_center.x + width:x = current_center.x - width;
    (index & 2) == 2? y = current_center.y + width:y = current_center.y - width;
    (index & 4) == 4? z = current_center.z + width:z = current_center.z - width;
    
    return Vector3D(x, y, z);
}

//============================================addChild===============================================================//
// Add Children to the current cell
void OctreeCell::addChild()
{
    childrens_ = new OctreeCell*[8];
    for (int i = 0; i < 8; i ++) {
        childrens_[i] = NULL;
    }

    for (int i = 0; i < 8; i++) {
        childrens_[i] = new OctreeCell();
        // add parent
        childrens_[i]->parent_ = this;
        // add depth
        childrens_[i]->depth_ = depth_ + 1;
    }

    // set the global and local offsets of children
    for(int i=0; i< 8; i++){
        childrens_[i]->initGlobalOffsets();
        childrens_[i]->initLocalOffsets();
    }
}

//================================================CornerIndex=========================================================//
// Return the index of a node according to it's parent, maybe the function name should be changed to avoid ambiguity
// Return the index of a corner that is close to a given child node,
// if child_node is root node, return -1.
int OctreeCell::CornerIndex(OctreeCell *child_node)
{
    // If node is parent
    if (child_node->parent_ == NULL)
        return 0;
    
    OctreeCell *parent = child_node->parent_;
    
    // If the child_node is root node, return -1
    if (parent == NULL)
        return -1;
    
    for (int i = 0; i < 8; i++)    {
        OctreeCell *cell = parent->childrens_[i];
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

    if (neighbor_cache_ != NULL) { delete neighbor_cache_; };

    // Initialize the neighborCache ptr, TODO[Luwei](better initialization method for ptr array)
    neighbor_cache_ = new OctreeCell *[27];
    for (int i = 0; i < 27; ++i) {
        neighbor_cache_[i] = NULL;
    }

    neighbor_cache_[NEIBORINDEX(1, 1, 1)] = this;

    if (parent_ == NULL) return;

    OctreeCell *parent = this->parent_;
    int child_index = this->getCornerIndexOfParent();

    // Get dimension offset (x,y,z) for the current node and the diagonal node
    int x1, y1, z1, x2, y2, z2;
    DimensionIndices(child_index, x1, y1, z1);
    DimensionIndices(DIAGONALINDEX(child_index), x2, y2, z2);

    // Add the siblings to the neighborCache
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                int index = CornerIndex(i, j, k);
                neighbor_cache_[NEIBORINDEX(x2 + i, y2 + j, z2 + k)] = parent->childrens_[index];
            }
        }
    }

    // Get the parents neighborCache
    OctreeCell **parent_neighbors = NULL;
    if (parent->neighbor_cache_ == NULL)   //if parent has no neighborCache cache, initialize the neighborCache
        parent->initNeighbors();
    parent_neighbors = parent->neighbor_cache_;

    // Set the neighborCache across the face
    int i, j, k;
    i = x1 << 1;
    OctreeCell *adj_face_node1 = parent_neighbors[NEIBORINDEX(i, 1, 1)];
    if (adj_face_node1 != NULL) {
        if (adj_face_node1->childrens_ == NULL) { adj_face_node1->addChild(); }
        for (j = 0; j < 2; j++) {
            for (k = 0; k < 2; k++) {
                neighbor_cache_[NEIBORINDEX(i, y2 + j, z2 + k)] =
                        adj_face_node1->childrens_[CornerIndex(x2, j, k)];
            }
        }
    }

    j = y1 << 1;
    OctreeCell *adj_face_node2 = parent_neighbors[NEIBORINDEX(1, j, 1)];
    if (adj_face_node2 != NULL) {
        if (adj_face_node2->childrens_ == NULL) { adj_face_node2->addChild(); }
        for (i = 0; i < 2; i++) {
            for (k = 0; k < 2; k++) {
                neighbor_cache_[NEIBORINDEX(x2 + i, j, z2 + k)] =
                        adj_face_node2->childrens_[CornerIndex(i, y2, k)];
            }
        }
    }

    k = z1 << 1;
    OctreeCell *adj_face_node3 = parent_neighbors[NEIBORINDEX(1, 1, k)];
    if (adj_face_node3 != NULL) {
        if (adj_face_node3->childrens_ == NULL) { adj_face_node3->addChild(); }
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 2; j++) {
                neighbor_cache_[NEIBORINDEX(x2 + i, y2 + j, k)] =
                        adj_face_node3->childrens_[CornerIndex(i, j, z2)];
            }
        }
    }

    // Set the neighborCache across the edge
    i = x1 << 1;
    j = y1 << 1;
    OctreeCell *adj_edge_node1 = parent_neighbors[NEIBORINDEX(i, j, 1)];
    if (adj_edge_node1 != NULL) {
        if (adj_edge_node1->childrens_ == NULL) { adj_edge_node1->addChild(); }
        for (k = 0; k < 2; k++) {
            neighbor_cache_[NEIBORINDEX(i, j, z2 + k)] =
                    adj_edge_node1->childrens_[CornerIndex(x2, y2, k)];
        }
    }

    i = x1 << 1;
    k = z1 << 1;
    OctreeCell *adj_edge_node2 = parent_neighbors[NEIBORINDEX(i, 1, k)];
    if (adj_edge_node2 != NULL) {
        if (adj_edge_node2->childrens_ == NULL) { adj_edge_node2->addChild(); }
        for (j = 0; j < 2; j++) {
            neighbor_cache_[NEIBORINDEX(i, y2 + j, k)] =
                    adj_edge_node2->childrens_[CornerIndex(x2, j, z2)];
        }
    }

    j = y1 << 1;
    k = z1 << 1;
    OctreeCell *adj_edge_node3 = parent_neighbors[NEIBORINDEX(1, j, k)];
    if (adj_edge_node3 != NULL) {
        if (adj_edge_node3->childrens_ == NULL) { adj_edge_node3->addChild(); }
        for (i = 0; i < 2; i++) {
            neighbor_cache_[NEIBORINDEX(x2 + i, j, k)] =
                    adj_edge_node3->childrens_[CornerIndex(i, y2, z2)];
        }
    }

    // Set neighborCache across the corner
    i = x1 << 1;
    j = y1 << 1;
    k = z1 << 1;
    OctreeCell *adj_corner_node = parent_neighbors[NEIBORINDEX(i, j, k)];
    if (adj_corner_node != NULL) {
        if (adj_corner_node->childrens_ == NULL) { adj_corner_node->addChild(); }
        neighbor_cache_[NEIBORINDEX(i, j, k)] = adj_corner_node->childrens_[CornerIndex(x2, y2, z2)];
    }

}

// Return the neighbor cell based on offsets (x,y,z)
OctreeCell *OctreeCell::neighbor(int x, int y, int z) {
    if (neighbor_cache_ == NULL) initNeighbors();
    return neighbor_cache_[NEIBORINDEX(x, y, z)];
}

// Return the neighbor cell based on neighbor index [0, 27]
OctreeCell *OctreeCell::neighbor(int index) {
    if (neighbor_cache_ == NULL)
        initNeighbors();
    return neighbor_cache_[index];
}

// Return the neighbors array ptr
OctreeCell **OctreeCell::neighbors() {
    if (neighbor_cache_ == NULL)
        initNeighbors();
    return neighbor_cache_;
}
