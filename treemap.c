#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
      TreeMap* map = (TreeMap*)malloc(sizeof(TreeMap));
    if (map == NULL) {
      return NULL;
    }

    map->lower_than = lower_than;
    map->root = NULL;
    map->current = NULL;

    return map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  if (tree == NULL) {
    return;
  }

  if (searchTreeMap(tree, key) != NULL) {
    return;
  }

  TreeNode* newNode = createTreeNode(key, value);

  if (tree->root == NULL) {
    tree->root = newNode;
  } else {
    TreeNode* current = tree->root;
    while (1) {
      if (tree->lower_than(key, current->pair->key) < 0) {
        if (current->left == NULL) {
          current->left = newNode;
          newNode->parent = current;
          break;
        } else {
          current = current->left;
        }
      } else {
        if (current->right == NULL) {
          current->right = newNode;
          newNode->parent = current;
          break;
        } else {
          current = current->right;
        }
      }
    }
  }
  
  tree->current = newNode;

}

TreeNode * minimum(TreeNode * x){
  if (x == NULL) {
    return NULL;
  }
  while (x->left != NULL) {
    x = x->left;
  } 
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
  if (tree == NULL || node == NULL) {
    return;
  }

  TreeNode* parent = node->parent;

  if (node->left == NULL && node->right == NULL) {
    if (parent != NULL) {
      if (parent->left == node) {
        parent->left = NULL;
      } else {
        parent->right = NULL;
      }
    } else {
      tree->root = NULL;
    }
    free(node->pair);
    free(node);
  } else if (node->left != NULL && node->right != NULL) {
      TreeNode* minRight = minimum(node->right);
      node->pair = minRight->pair;
      removeNode(tree, minRight);
  } else {  
    TreeNode* child = (node->left != NULL) ? node->left : node->right;
    if (parent != NULL) {
      if (parent->left == node) {
        parent->left = child;
      } else {
        parent->right = child;
      }
      child->parent = parent;
    } else {
      tree->root = child;
      if (child != NULL) {
        child->parent = NULL;
      }
    }
    free(node->pair);
    free(node);
  }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) 
      return NULL;

    TreeNode* current = tree->root;
    while (current != NULL) {
      int cmp = tree->lower_than(key, current->pair->key);
      if (cmp == 0) {
        tree->current = current;
        return current->pair;
      } else if (cmp < 0) {
            current = current->left;
      } else {
          current = current->right;
      }
    }

  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
  if (tree == NULL || tree->root == NULL) {
    return NULL;
  }
  
  TreeNode* current = tree->root;
  TreeNode* ub_node = NULL;

  while (current != NULL) {
    int cmp = tree->lower_than(key, current->pair->key);
    if (cmp == 0) {
      tree->current = current;
      return current->pair;
    } else if (cmp < 0) {
        ub_node = current;
        current = current->left;
    } else {
      current = current->right;
    }
  }
  
  if (ub_node != NULL) {
    tree->current = ub_node;
    return ub_node->pair;
  }
  
  return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
  if (tree == NULL || tree->root == NULL) {
    return NULL;
  }

  TreeNode* current = tree->root;
  while (current->left != NULL) {
    current = current->left;
  }

  tree->current = current;
  return current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  if (tree == NULL || tree->current == NULL) {
    return NULL;
  }

  TreeNode* current = tree->current;

  if (current->right != NULL) {
    current = current->right;
    while (current->left != NULL) {
      current = current->left;
    }
    tree->current = current;
    return current->pair;
  } else {
    TreeNode* parent = current->parent;
    while (parent != NULL && current == parent->right) {
      current = parent;
      parent = parent->parent;
    }
    tree->current = parent;
    return (parent != NULL) ? parent->pair : NULL;
  }
}
