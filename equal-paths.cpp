#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
static int checkDepth(Node* n){

  if(n == nullptr){
    return 0;
  }

  if(n->left == nullptr && n->right == nullptr){
    return 1;
  }

  if(n->left == nullptr){
    int rightD = checkDepth(n->right);
    if(rightD == -1) {
      return -1;
    }
    return rightD+1;
  }

  if(n->right == nullptr){
    int leftD = checkDepth(n->left);
    if(leftD == -1) {
      return -1;
    }
    return leftD+1;
  }

  else{
    int leftD = checkDepth(n->left);
    int rightD = checkDepth(n->right);
    if(leftD == -1 || rightD == -1) return -1;
    if(leftD != rightD) return -1;


    return leftD+1;
  }
  
}

bool equalPaths(Node * root)
{
    // Add your code below
    return checkDepth(root) != -1;
}

