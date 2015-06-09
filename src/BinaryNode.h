//
//  BinaryNode.h
//  Reconstructor
//
//  Created by Luwei Yang on 6/10/15.
//  Copyright (c) 2015 Luwei Yang. All rights reserved.
//

#ifndef __Reconstructor__BinaryNode__
#define __Reconstructor__BinaryNode__

class BinaryNode {
    
    // pointer to the parent
    BinaryNode *parent_;
    
    // pointer to the sibling
    BinaryNode *sibling_;
    
    // pointer to the childrens
    BinaryNode *leftChild_;
    BinaryNode *rightChild_;
    
public:
    // Getter of childrens
    BinaryNode* leftChild();
    BinaryNode* rightChild();
    
    // Getter of parent
    BinaryNode* parent();
    
    // Check if is leaf
    bool isLeaf();
    
    // Add Childrens
    void addChild();
    
};

#endif /* defined(__Reconstructor__BinaryNode__) */
