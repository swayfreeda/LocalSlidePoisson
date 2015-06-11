//
//  BinaryTree.cpp
//  Reconstructor
//
//  Created by Luwei Yang on 6/10/15.
//  Copyright (c) 2015 Luwei Yang. All rights reserved.
//

#include <BinaryTree.h>
#include <BinaryNode.h>


BinaryTree::BinaryTree() {

    // Allocate root node
    root_ = new BinaryNode();

}

BinaryTree::~BinaryTree() {

    // First destory the tree via deallocate()
    deallocate();

    // Set ptr to null
    this->root_ = NULL;

    // Deal with miscellaneious things
    this->depth_ = 0;
}

void BinaryTree::deallocate() {
    std::queue<BinaryNode*> delCollections;

    // Find first leaf and push it to stack
    delCollections.push(root_);

    BinaryNode* cell = NULL;

    // Iterate the cell from root to its childrens to destory
    // binary tree
    while (!delCollections.empty())
    {
        cell = delCollections.front();
        delCollections.pop();

        // Push the children to stack
        if (!cell->isLeaf()) {
            BinaryNode *leftChild = cell->leftChild();
            BinaryNode *rightChild = cell->rightChild();
            if (leftChild != NULL) delCollections.push(leftChild);
            if (rightChild != NULL) delCollections.push(rightChild);
        }

        // Delete current octree cell
        delete cell;
        cell = NULL;
    }

    this->root_ = NULL;
}


long long BinaryTree::nodeCounts() {
    std::queue<BinaryNode*> collections;

    // Find first leaf and push it to stack
    collections.push(root_);
    long long count = 1;

    // Iterate the cell from root to its childrens to destory
    // binary tree
    BinaryNode* cell = NULL;
    while (!collections.empty())
    {
        cell = collections.front();
        collections.pop();

        // Push the children to stack
        if (!cell->isLeaf()) {
            BinaryNode *leftChild = cell->leftChild();
            BinaryNode *rightChild = cell->rightChild();
            if (leftChild != NULL) collections.push(leftChild);
            if (rightChild != NULL) collections.push(rightChild);
        }

        // Count the current node
        count++;
    }

    return count;
}

// Get the begin iterator
BinaryTree::NodeIterator BinaryTree::begin()
{
    return BinaryTree::NodeIterator(this->root_);
}

// Get the end iterator
BinaryTree::NodeIterator BinaryTree::end()
{
    return BinaryTree::NodeIterator(NULL);
}

// Default constructor of iterator, init with NULL ptr
BinaryTree::NodeIterator::NodeIterator()
        :currentNode(NULL)
{
}

// Constructor of iterator with exist cell as beginer
BinaryTree::NodeIterator::NodeIterator(BinaryNode *cell)
        :currentNode(cell)
{
    if(cell != NULL)
    {
        // Enqueue the cells children
        if (!currentNode->isLeaf()) {
            BinaryNode *leftChild = currentNode->leftChild();
            BinaryNode *rightChild = currentNode->rightChild();
            if (leftChild != NULL) iterationQueue.push(leftChild);
            if (rightChild != NULL) iterationQueue.push(rightChild);
        }
    }
}

// Iterate next cell
BinaryTree::NodeIterator& BinaryTree::NodeIterator::operator++(int)
{
    // Check if have iterated to the end
    if (iterationQueue.empty())
    {
        currentNode = NULL;
        return (*this);
    }

    currentNode = iterationQueue.front();
    iterationQueue.pop();

    // Enqueue the children
    if (!currentNode->isLeaf()) {
        BinaryNode *leftChild = currentNode->leftChild();
        BinaryNode *rightChild = currentNode->rightChild();
        if (leftChild != NULL) iterationQueue.push(leftChild);
        if (rightChild != NULL) iterationQueue.push(rightChild);
    }

    return (*this);
}
