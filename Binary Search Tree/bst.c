/*
 * This file is where you should implement your binary search tree.  It already
 * contains skeletons of the functions you need to implement (along with
 * documentation for each function).  Feel free to implement any additional
 * functions you might need.  Also, don't forget to include your name and
 * @oregonstate.edu email address below.
 *
 * Name:
 * Email:
 */

#include <stdlib.h>
#include <stdio.h>

#include "bst.h"

/*
 * This structure represents a single node in a BST.  In addition to containing
 * pointers to its two child nodes (i.e. `left` and `right`), it contains two
 * fields representing the data stored at this node.  The `key` field is an
 * integer value that should be used as an identifier for the data in this
 * node.  Nodes in the BST should be ordered based on this `key` field.  The
 * `value` f_nield stores data associated with the key.
 *
 * You should not modify this structure.
 */
struct bst_node {
  int key;
  void* value;
  struct bst_node* left;
  struct bst_node* right;
};


/*
 * This structure represents an entire BST.  It specifically contains a
 * reference to the root node of the tree.
 *
 * You should not modify this structure.
 */
struct bst {
  struct bst_node* root;
};

/*
 * This function should allocate and initialize a new, empty, BST and return
 * a pointer to it.
 */
struct bst* bst_create() {
  struct bst* new_bst = malloc(sizeof(struct bst));
  new_bst->root = NULL;
  return new_bst;
}

/*
 *This function traverses the binary tree processing the value at last contact, and it is done recursively
 */
void trav_free(struct bst_node* nav) {
  if(nav != NULL) {
    trav_free(nav->left);
    trav_free(nav->right);
    free(nav);
  } else {
    return;
  }
  return;
}

/*
 * This function should free the memory associated with a BST.  While this
 * function should up all memory used in the BST itself, it should not free
 * any memory allocated to the pointer values stored in the BST.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   bst - the BST to be destroyed.  May not be NULL.
 */
void bst_free(struct bst* bst) {
  //esentially just have to traverse the list so go to his slides on that
  trav_free(bst->root);
  bst->root = NULL;
  free(bst);
  return;
}

int trav_count(struct bst_node* nav) {
  if(nav != NULL) {
    return 1 + trav_count(nav->left) + trav_count(nav->right);
  } else {
    return 0;
  }
}

/*
 * This function should return the total number of elements stored in a given
 * BST.
 *
 * Params:
 *   bst - the BST whose elements are to be counted.  May not be NULL.
 */
int bst_size(struct bst* bst) {
  //esentially just have to traverse the list so go to his slides on that
  int size;
  size = trav_count(bst->root);
  return (size);
}

/*
 * This function should insert a new key/value pair into the BST.  The key
 * should be used to order the key/value pair with respect to the other data
 * stored in the BST.  The value should be stored along with the key, once the
 * right location in the tree is found.
 *
 * Params:
 *   bst - the BST into which a new key/value pair is to be inserted.  May not
 *     be NULL.
 *   key - an integer value that should be used to order the key/value pair
 *     being inserted with respect to the other data in the BST.
 *   value - the value being inserted into the BST.  This should be stored in
 *     the BST alongside the key.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void bst_insert(struct bst* bst, int key, void* value) {
  struct bst_node* nav = bst->root;
  struct bst_node* parent; 
  struct bst_node* new_node = malloc(sizeof(struct bst_node));
  
  new_node->key = key;
  new_node->value = value;

  if(bst->root == NULL) {
    bst->root = new_node;
  } else {
    while(nav != NULL) {
      parent = nav;
      if(key < nav->key) {
        nav = nav->left;
      } else {
        nav = nav->right;
      }
    }
    if(key < parent->key) {
      parent->left = new_node;
    } else {
      parent->right = new_node;
    }
  }

  return;
}

void delete_onec(struct bst_node* parent, struct bst_node* cur, int key) {
  struct bst_node* child;

  if(cur->left != NULL) {
    child = cur->left;
  } else {
    child = cur->right;
  }
  free(cur);
  if(key < parent->key) {
    parent->left = child;
  } else {
    parent->right = child;
  }
}

void delete_twoc(struct bst* bst, struct bst_node* parent, struct bst_node* cur) {
  int side = 0;
  struct bst_node* succ = cur->right;
  struct bst_node* succ_par;
  struct bst_node* succ_kid = NULL;

  while(succ->left != NULL) {
    succ_par = succ;
    succ = succ->left;
  }

  if(parent == NULL) {
    if(succ->right != NULL) {
      succ_par->left = succ->right;
    } else {
      succ_par->left = NULL;
    }
    succ->left = cur->left;
    succ->right = cur->right;
    free(cur);
    bst->root = succ;
  
  } else {
    //To find if we are on the right or left side of the parent
    if(parent->right == cur) {
      side = 1;
    }

    //If they are right next to each other then special things must be taken into consideration
    if(cur->right == succ) {
      if(side == 0) {
        parent->left = succ;
      } else {
        parent->right = succ;
      }
      succ->left = cur->left;
      free(cur);
    } else {
      if(succ->right != NULL) {
        succ_par->left = succ->right;
      } else {
        succ_par->left = NULL;
      }
      succ->left = cur->left;
      succ->right = cur->right;
      free(cur);
    }
  }
  return;
}

void delete_noc(struct bst_node* parent, struct bst_node* cur, int key) {
  if(cur == parent->left) {
    free(cur);
    parent->left = NULL;
  } else if (cur == parent->right){
    free(cur);
    parent->right = NULL;
  }
}

/*
 * This function should remove a key/value pair with a specified key from a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should remove the first one it encounters (i.e. the one closest to
 * the root of the tree).
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair to be removed from the BST.
 */
void bst_remove(struct bst* bst, int key) {
  struct bst_node* nav = bst->root;
  struct bst_node* parent = NULL;
  
  while(nav != NULL) {
    if(key == nav->key) {
      break;
    }
    parent = nav;
    if(key < nav->key) {
      nav = nav->left;
    } else {
      nav = nav->right;
    }
  }

  if(nav->right != NULL && nav->left != NULL) {
    //Remove function with two children
    //First must find the in order successor
    //printf("call delete two children nav: %d \n", nav->key);
    //printf("parent key: %d \n", parent->key);
    delete_twoc(bst, parent, nav); //must consider what the parent will be when its the root
  } else if(nav->right == NULL && nav->left == NULL) {
    //Remove function with no children
    //printf("parent key: %d \n", parent->key);
    //printf("call delete no children nav: %d \n", nav->key);
    delete_noc(parent, nav, key);
  } else {
    //Remove function with one children
    //printf("parent key: %d \n", parent->key);
    //printf("call delete one children nav: %d \n", nav->key);
    delete_onec(parent, nav, key);
  }

  return;
}

/*
 * This function should return the value associated with a specified key in a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should return the first one it encounters (i.e. the one closest to
 * the root of the tree).  If the BST does not contain the specified key, this
 * function should return NULL.
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair whose value is to be returned.
 *
 * Return:
 *   Should return the value associated with the key `key` in `bst` or NULL,
 *   if the key `key` was not found in `bst`.
 */
void* bst_get(struct bst* bst, int key) {
  struct bst_node* nav = bst->root;

  while(nav != NULL) {
    //printf("key: %d, nav->key: %d\n", key, nav->key);
    if(key == nav->key) {
      return nav->value;
    } else if (key < nav->key) {
      nav = nav->left;
    } else if (key > nav->key) {
      nav = nav->right;
    }
  }
  return NULL;
}
