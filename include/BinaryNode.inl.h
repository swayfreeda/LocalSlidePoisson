//
// Created by Luwei Yang on 6/11/15.
//

#ifndef RECONSTRUCTOR_BINARYNODE_INL_H
#define RECONSTRUCTOR_BINARYNODE_INL_H

#include "BinaryNode.h"
#include <iostream>

BinaryNode* BinaryNode::leftChild() {
    return this->left_child_;
}

BinaryNode* BinaryNode::rightChild() {
    return this->right_child_;
}

BinaryNode *BinaryNode::parent() {
    return this->parent_;
}

bool BinaryNode::isLeaf() {
    return (left_child_ == NULL && right_child_ == NULL);
}


#endif //RECONSTRUCTOR_BINARYNODE_INL_H
