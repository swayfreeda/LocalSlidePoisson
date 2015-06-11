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
        parent_(nullptr),
        leftChild_(nullptr),
        rightChild_(nullptr),
        depth_(0)
{
}

BinaryNode::~BinaryNode() {

    // Note that we will not destory childrens
    // and parent in destructor
    if (leftChild_ != nullptr)
        leftChild_ = nullptr;

    if (rightChild_ != nullptr)
        rightChild_ = nullptr;

    if (parent_ != nullptr)
        parent_ = nullptr;

    depth_ = 0;
}

void BinaryNode::addChild() {
    assert(leftChild_ != nullptr);
    assert(rightChild_!= nullptr);

    leftChild_ = new BinaryNode();
    leftChild_->parent_ = this;
    leftChild_->depth_ = this->depth_ + 1;

    rightChild_ = new BinaryNode();
    rightChild_->parent_ = this;
    rightChild_->depth_ = this->depth_ + 1;
}
