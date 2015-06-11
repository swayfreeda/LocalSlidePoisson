//
//  BinaryNode.cpp
//  Reconstructor
//
//  Created by Luwei Yang on 6/10/15.
//  Copyright (c) 2015 Luwei Yang. All rights reserved.
//

#include <BinaryNode.h>
#include <assert.h>
#include <iostream>


BinaryNode::BinaryNode():
        parent_(NULL),
        leftChild_(NULL),
        rightChild_(NULL),
        depth_(0)
{
}

BinaryNode::~BinaryNode() {

    // Note that we will not destory childrens
    // and parent in destructor
    if (leftChild_ != NULL)
        leftChild_ = NULL;

    if (rightChild_ != NULL)
        rightChild_ = NULL;

    if (parent_ != NULL)
        parent_ = NULL;

    depth_ = 0;
}

void BinaryNode::addChild() {
    assert(leftChild_ != NULL);
    assert(rightChild_!= NULL);

    leftChild_ = new BinaryNode();
    leftChild_->parent_ = this;
    leftChild_->depth_ = this->depth_ + 1;

    rightChild_ = new BinaryNode();
    rightChild_->parent_ = this;
    rightChild_->depth_ = this->depth_ + 1;
}
