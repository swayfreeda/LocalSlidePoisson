//
//  BinaryNode.cpp
//  Reconstructor
//
//  Created by Luwei Yang on 6/10/15.
//  Copyright (c) 2015 Luwei Yang. All rights reserved.
//

#include "BinaryNode.h"
#include <assert.h>

BinaryNode::BinaryNode():
        parent_(NULL),
        left_child_(NULL),
        right_child_(NULL),
        depth_(0)
{
}

BinaryNode::~BinaryNode() {

    // Note that we will not destory childrens
    // and parent in destructor
    if (left_child_ != NULL)
        left_child_ = NULL;

    if (right_child_ != NULL)
        right_child_ = NULL;

    if (parent_ != NULL)
        parent_ = NULL;

    depth_ = 0;
}

void BinaryNode::addChildren() {
    
    if (left_child_ != NULL ||
        right_child_ != NULL)
        return;
    
    
    left_child_ = new BinaryNode();
    left_child_->parent_ = this;
    left_child_->depth_ = this->depth_ + 1;

    right_child_ = new BinaryNode();
    right_child_->parent_ = this;
    right_child_->depth_ = this->depth_ + 1;
}
