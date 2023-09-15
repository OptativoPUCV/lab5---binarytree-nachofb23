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
  if (tree == NULL || key == NULL || value == NULL) {
    return;
  }

  TreeNode* newNode = createTreeNode(key, value);
  if (newNode == NULL) {
    return;
  }

  if (tree->root == NULL) {
    tree->root = newNode;
    tree->current = newNode;
  } else {
    TreeNode* current = tree->root;
    TreeNode* parent = NULL;

    while (current != NULL) {
      parent = current;
      int cmp = tree->lower_than(key, current->pair->key);
      if (cmp == 0) {
        free(newNode->pair);
        free(newNode);
        return;
      } else if (cmp < 0) {
        current = current->left;
      } else {
        current = current->right;
      }
    }

    newNode->parent = parent;
    int cmp = tree->lower_than(key, parent->pair->key);
    if (cmp < 0) {
      parent->left = newNode;
    } else {
      parent->right = newNode;
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
  return;
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* currentNodo = tree->root;
  
    while (currentNodo != NULL) {
      int cmp = is_equal(tree, key, currentNodo->pair->key);
      if (cmp) {
        tree->current = currentNodo;
        return currentNodo->pair;
      } else if (tree->lower_than(key, currentNodo->pair->key) > 0) {
            currentNodo = currentNodo->left;
      } else {
          currentNodo = currentNodo->right;
      }
    }
  tree->current = NULL;
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
