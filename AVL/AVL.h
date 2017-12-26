#ifndef AVL_h
#define AVL_h
#pragma once

#include "NodeInterface.h"
#include "AVLInterface.h"
#include "Node.h"
#include <iostream>

using namespace std;

class AVL : public AVLInterface {
public:
    Node* root;
    
    AVL() {
        root = NULL;
    }
    
    ~AVL() {
        clear();
    }
    
    //Please note that the class that implements this interface must be made
    //of objects which implement the NodeInterface
    
    /*
     * Returns the root node for this tree
     *
     * @return the root node for this tree.
     */
    NodeInterface * getRootNode() const {
        return root;
    }
    
    /*
     * Attempts to add the given int to the AVL tree
     * Rebalances the tree if data is successfully added
     *
     * @return true if added
     * @return false if unsuccessful (i.e. the int is already in tree)
     */
    bool add(int data) {
        //cout << "---- Adding " << data << " ----" << endl;
        return insert(root, data);
    }
    
    bool insert (Node* &T, int val) {
        if (T == NULL) {
            T = new Node(val);
            return true;
        }
        
        if (T->value == val) {
            //cout << val << " not added" << endl;
            return false;
        }

        if (val > T->value) {
            bool result = insert(T->right, val);
            
            if (result) {
                T->rightHeight = updateRightHeight(T);
                
                if ( !isBalanced(T) ) {
                   // cout << T->value << " is Not balanced. " << endl;
                    rotate(T, findCondition(T));
                }
            }
            
            return result;
        }
        
        if (val < T->value) {
            bool result = insert(T->left, val);
            
            if (result) {
                T->leftHeight = updateLeftHeight(T);
                
                if ( !isBalanced(T) ) {
                   // cout << T->value << " is Not balanced. " << endl;
                    rotate(T, findCondition(T));
                }
            }
            
            return result;
        }
        
        // We should never get to this line.
        return false;
    }
    
    void rotateLeft (Node* &T) {
        Node* oldRoot = T;
        Node* newRoot = T->right;
        
        T = newRoot;
        oldRoot->right = newRoot->left;
        newRoot->left = oldRoot;
        
        oldRoot->rightHeight = updateRightHeight(oldRoot);
        oldRoot->leftHeight = updateLeftHeight(oldRoot);
        T->leftHeight = updateLeftHeight(T);
    }
    
    void rotateRight (Node* &T) {
        Node* oldRoot = T;
        Node* newRoot = T->left;
        
        T = newRoot;
        oldRoot->left = newRoot->right;
        newRoot->right = oldRoot;
        
        oldRoot->rightHeight = updateRightHeight(oldRoot);
        oldRoot->leftHeight = updateLeftHeight(oldRoot);
        T->rightHeight = updateRightHeight(T);
    }
    
    void rotate (Node* &T, int condition) {
        switch (condition) {
            case 0 : // left-left
                rotateRight(T);
                break;
            case 1 : // left-right
                rotateLeft(T->left);
                rotateRight(T);
                break;
            case 2 : // right-left
                rotateRight(T->right);
                rotateLeft(T);
                break;
            case 3 : // right-right
                rotateLeft(T);
                break;
        }
    }
    
    int findCondition (Node* T) {
        // conditions:
        // 0 left-left
        // 1 left-right
        // 2 right-left
        // 3 right-right
        
        if ( T->leftHeight > T->rightHeight) {
            if (T->left->leftHeight >= T->left->rightHeight) {
                //cout << "Left-Left" << endl;
                return 0;
            }
            else {
                //cout << "Left-Right" << endl;
                return 1;
            }
        }
        else {
            if (T->right->leftHeight > T->right->rightHeight) {
                //cout << "Right-Left" << endl;
                return 2;
            }
            else {
                //cout << "Right-Right" << endl;
                return 3;
            }
        }
    }
    
    int updateRightHeight(Node* T) {
        if (T->right == NULL)
            return 0;
        
        return 1 + max(T->right->leftHeight, T->right->rightHeight);
    }
    
    int updateLeftHeight(Node* T) {
        if (T->left == NULL)
            return 0;
        
        return 1 + max(T->left->leftHeight, T->left->rightHeight);
    }
    
    int max(int a, int b) {
        return a > b ? a : b;
    }
    
    bool isBalanced (Node* T) {
        return abs(T->rightHeight - T->leftHeight) <= 1;
    }
    
    /*
     * Attempts to remove the given int from the AVL tree
     * Rebalances the tree if data is successfully removed
     *
     * @return true if successfully removed
     * @return false if remove is unsuccessful(i.e. the int is not in the tree)
     */
    bool remove(int val) {
        cout << "Removing " << val << endl;
        return remove(root, val);
    }
    
    bool remove(Node*& T, int val) {
        
        if (T == NULL) {
            return false;
        }
        
        if (val < T->value) {
            bool result = remove(T->left, val);
            
            if (result) {
                T->leftHeight = updateLeftHeight(T);
                T->rightHeight = updateRightHeight(T);
                
                if (!isBalanced(T)) {
                    rotate(T, findCondition(T));
                }
            }
            
            return result;
        }
        else if (val > T->value) {
            bool result = remove(T->right, val);
            
            if (result) {
                T->leftHeight = updateLeftHeight(T);
                T->rightHeight = updateRightHeight(T);
                
                if (!isBalanced(T)) {
                    rotate(T, findCondition(T));
                }
            }
            
            return result;
        }
        else {
            if (T->left == NULL && T->right == NULL) {
                delete T;
                T = NULL;
                return true;
            }
            else if (T->left != NULL && T->right != NULL) {
                int n = getHighest(T->left);
                remove(T->left, n);
                T->value = n;
                
                T->leftHeight = updateLeftHeight(T);
                
                if (!isBalanced(T))
                    rotate(T, findCondition(T));
                
                return true;
            }
            else if (T->left != NULL) {
                Node* temp = T;
                T = T->left;
                delete temp;
                return true;
            }
            else if (T->right != NULL) {
                Node* temp = T;
                T = T->right;
                delete temp;
                return true;
            }
        }
        
        return false;
    }
    
    int getHighest(Node* T) {
        if (T->right == NULL) {
            return T->value;
        }
        else
            return getHighest(T->right);
    }
    
    void clear (Node*& T) {
        if (T == NULL)
            return;
        
        clear(T->left);
        clear(T->right);
        
        delete T;
        T = NULL;
    }
    
    /*
     * Removes all nodes from the tree, resulting in an empty tree.
     */
    void clear() {
        clear(root);
    }
};

#endif /* AVL_h */
