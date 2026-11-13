#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
   
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here

    void insertFix(AVLNode<Key, Value>* p, AVLNode<Key,Value>*n);
    void removeFix(AVLNode<Key, Value>* n, int8_t diff);
    void rotateLeft(AVLNode<Key, Value>* n);
    void rotateRight(AVLNode<Key,Value>* n);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this->root_ == nullptr){
      this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
      return;
    } 

    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this ->root_);
    while(true){
      if(new_item.first == curr->getKey()){
        curr->setValue(new_item.second);
        return;
      }
      else if(new_item.first < curr->getKey()){
        if(curr->getLeft() == nullptr){
          AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, curr);
          curr->setLeft(newNode);
          curr->updateBalance(-1);
          if(curr->getBalance() != 0){
            insertFix(curr, newNode);
          }
          return;
        }
        curr = curr ->getLeft();
      }
      else{
        if(curr->getRight() == nullptr){
          AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, curr);
          curr->setRight(newNode);
          curr->updateBalance(1);
          if(curr->getBalance() !=0 ){
            insertFix(curr, newNode);
          }
          return;
        }
        curr = curr ->getRight();
      }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix (AVLNode<Key, Value>* p, AVLNode<Key,Value>* n){
  if(p == nullptr) return;
  AVLNode<Key,Value>* g = p ->getParent();
  if(g == nullptr) return;

  if(g->getLeft() == p){
    g->updateBalance(-1);
  }


  else{
    g->updateBalance(1);
  }

  int8_t gbss = g->getBalance();
  if(gbss == 0) return;
  else if(gbss == -1 || gbss ==1){
    insertFix(g, p);
  }
  else if(gbss == -2){
    if(p ->getBalance() == -1){
      rotateRight(g);
      p->setBalance(0);
      g->setBalance(0);

    }
    else{
      int8_t nbs = n->getBalance();
      rotateLeft(p);
      rotateRight(g);
      if(nbs ==-1){
        p->setBalance(0);
        g->setBalance(1);
        n-> setBalance(0);
      }
      else if(nbs == 0){
        g->setBalance(0);
        p->setBalance(0);
        n-> setBalance(0);
      }
      else{
        p->setBalance(-1);
        g->setBalance(0);
        n-> setBalance(0);
      }
    }
  }
    else{
      if(p->getBalance()==1){
        rotateLeft(g);
        p->setBalance(0);
        g-> setBalance(0);
      }
      else{
        int8_t nb = n ->getBalance();
        rotateRight(p);
        rotateLeft(g);
        if(nb ==1){
          p->setBalance(0);
          g->setBalance(-1);
          n->setBalance(0);
        }
        else if(nb == 0){
          g->setBalance(0);
          p->setBalance(0);
          n->setBalance(0);
        }
        else{
          p->setBalance(1);
          g->setBalance(0);
          n->setBalance(0);
        }
      }
    }
  }



/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key,Value>*>(this->internalFind(key));
    if(node == nullptr) return ;

    if(node ->getLeft() != nullptr && node ->getRight() != nullptr){
      AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(node));
      nodeSwap(node, pred);
    }

    AVLNode<Key,Value>* parent = node->getParent();
    AVLNode<Key,Value>* child = nullptr;
    if(node -> getLeft() != nullptr){
      child = node->getLeft();
    }
    else{
      child = node->getRight();
    }

    int8_t difff = 0;
    if(parent != nullptr){
      if(parent ->getLeft() == node){
        difff = 1;
      }
      else{
        difff=-1;
      }
    }
    if(child != nullptr){
      child -> setParent(parent);
    }
    if(parent == nullptr){
      this->root_ = child;
    }
    else if(parent->getLeft() == node){
      parent->setLeft(child);
    }
    else{
      parent->setRight(child);
    }
    delete node;

    removeFix(parent, difff);
}

template<class Key, class Value>
void AVLTree<Key, Value>:: removeFix(AVLNode<Key, Value>* n , int8_t diff){
  if(n == nullptr) return;

  AVLNode<Key, Value>* parent = n -> getParent();
  int8_t notdiff =0;
  if(parent != nullptr){
    if(parent ->getLeft() == n){
      notdiff = 1;
    }
    else{
      notdiff = -1;
    }
  }

  int8_t newBalance = n->getBalance() + diff;

  if(newBalance == -2){
    AVLNode<Key,Value>* c = n->getLeft();
    if(c->getBalance()<=0){
      rotateRight(n);
      
        if(c->getBalance() ==0){
          n->setBalance(-1);
          c->setBalance(1);
          return;
        }
        else{
          n->setBalance(0);
          c->setBalance(0);
          removeFix(parent, notdiff);
        }
      }
      else{
        AVLNode<Key, Value>* g = c->getRight();
        rotateLeft(c);
        rotateRight(n);
        if(g->getBalance() ==1){
          n->setBalance(0);
          c->setBalance(-1);
          g->setBalance(0);
        }
        else if(g->getBalance() == 0){
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);
        }
        else{
          n->setBalance(1);
          c->setBalance(0);
          g->setBalance(0);
        }
        removeFix(parent, notdiff);
      }
    }
    else if(newBalance == 2) {
      AVLNode<Key, Value>* c = n->getRight();
      if(c->getBalance() >= 0){
        rotateLeft(n);
        if(c->getBalance() == 0){
          n->setBalance(1);
          c->setBalance(-1);
          return;
        }
        else{
          n->setBalance(0);
          c->setBalance(0);
          removeFix(parent , notdiff);
        }
      }
      else{
        AVLNode<Key, Value>* g = c->getLeft();
        rotateRight(c);
        rotateLeft(n);
        if(g->getBalance() == -1){
          n->setBalance(0);
          c->setBalance(1);
          g->setBalance(0);
        }
        else if(g->getBalance() == 0){
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);
        }
        else{
          n->setBalance(-1);
          c->setBalance(0);
          g->setBalance(0);
        }
        removeFix(parent, notdiff); 
      }
    }
    else if(newBalance == -1 || newBalance == 1){
      n->setBalance(newBalance);
    }
    else{
      n->setBalance(0);
      removeFix(parent, notdiff);
    }

  } 


template<class Key, class Value>
void AVLTree<Key, Value>:: rotateRight(AVLNode<Key,Value>* n){
  AVLNode<Key, Value>* leftChild = n->getLeft();
  AVLNode<Key, Value>* parent = n->getParent();

  n->setLeft(leftChild->getRight());
  if(leftChild->getRight() != nullptr){
    leftChild->getRight()->setParent(n);
  }

  leftChild->setRight(n);
  n->setParent(leftChild);

  leftChild->setParent(parent);
  if(parent==nullptr){
    this->root_=leftChild;
  }
  else if(parent->getLeft() == n){
    parent ->setLeft(leftChild);
  }
  else{
    parent->setRight(leftChild);
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>:: rotateLeft(AVLNode<Key,Value>* n){
  AVLNode<Key, Value>* rightChild = n->getRight();
  AVLNode<Key, Value>* parent = n->getParent();

  n->setRight(rightChild->getLeft());
  if(rightChild->getLeft() != nullptr){
    rightChild->getLeft()->setParent(n);
  }

  rightChild->setLeft(n);
  n->setParent(rightChild);

  rightChild->setParent(parent);
  if(parent==nullptr){
    this->root_=rightChild;
  }
  else if(parent->getLeft() == n){
    parent ->setLeft(rightChild);
  }
  else{
    parent->setRight(rightChild);
  }
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
