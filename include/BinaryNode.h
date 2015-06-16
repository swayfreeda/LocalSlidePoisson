//
//  BinaryNode.h
//  Reconstructor
//
//  Created by Luwei Yang on 6/10/15.
//  Copyright (c) 2015 Luwei Yang. All rights reserved.
//

#ifndef Reconstructor_BinaryNode_h
#define Reconstructor_BinaryNode_h


class BinaryNode
{
protected:
    // The ptr point to the parent node
    BinaryNode *parent_;

    // Ptrs point to childrens
    BinaryNode *left_child_;
    BinaryNode *right_child_;

    // The depth of the binary node
    int depth_;
    
public:

    // Initial constructor
    BinaryNode();

    // Destructor
    ~BinaryNode();

    // Get depth
    inline int depth();
    
    // Set the depth
    void setDepth(int depth){depth_ = depth;}
    
    // Set left child with new ptr
    void setLeftChild(BinaryNode *left_child);
    
    // Set right child with right ptr
    void setRightChild(BinaryNode *right_child);
    
    // Set parent ptr
    void setParent(BinaryNode *parent);
    
    // Create childrens branch
    void addChildren();

    // Return the ptr of childrens
    inline BinaryNode* leftChild();
    inline BinaryNode* rightChild();

    // Return the ptr of parent
    inline BinaryNode* parent();

    // Return true if current is leaf node
    inline bool isLeaf();
};

#include "BinaryNode.inl.h"

#endif
