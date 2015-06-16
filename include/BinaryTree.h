//
//  BinaryTree.h
//  Reconstructor
//
//  Created by Luwei Yang on 6/10/15.
//  Copyright (c) 2015 Luwei Yang. All rights reserved.
//

#ifndef Reconstructor_BinaryTree_h
#define Reconstructor_BinaryTree_h

#include "BinaryNode.h"
#include <queue>
#include <iostream>

class BinaryTree {
    
    // the ptr to the root node
    BinaryNode *root_;
    
    // the depth of the tree
    int depth_;
    
public:

    // Constructor
    BinaryTree();

    // Deconstructor
    ~BinaryTree();

    // Deallocate the octree, include the root node
    void deallocate();
    
    // return the ptr of root node
    inline BinaryNode* rootNode();
    
    // Count the cells, from root to its childrens
    long long nodeCounts();

    // Iterator, pre-order iterate the nodes
    class NodeIterator
    {
    public:
        // Default iterator with NULL ptr
        NodeIterator();
        
        // Default iterator
        NodeIterator(BinaryNode *cell);
        
        // Reload '++' operator for iterating next cell
        NodeIterator& operator ++ (int);
        
        // Get the cell ptr
        inline BinaryNode* operator *()
        {
            return current_node;
        }
        
        // Treat the iterator as OctreeCell ptr
        inline BinaryNode* operator ->()
        {
            return current_node;
        }
        
        inline bool operator == (const NodeIterator & rhs) const
        {
            return ((current_node != NULL)? this->current_node != rhs.current_node : rhs.current_node == NULL);
        }
        
        inline bool operator != (const NodeIterator & rhs) const
        {
            // Currently we compare the ptr address
            return ((current_node != NULL)? this->current_node != rhs.current_node : rhs.current_node != NULL);
        }
        
    private:
        std::queue<BinaryNode*> iteation_queue;
        BinaryNode* current_node;
    };

    // Iterator methods
    NodeIterator begin();
    NodeIterator end();

};

#include "BinaryTree.inl.h"

#endif
