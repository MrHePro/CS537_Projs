////////////////////////////////////////////////////////////////////////////////
// Main File:        N/A
// This File:        rang_tree.c
// Other Files:      README, makefile, 537malloc.h,
// 					 537malloc.c, range_tree.h
//                   
// Semester:         CS 537 Fall 2018
//
// Author:           Zihao Song, Dong He
// Email:            zsong67@wisc.edu, dhe28@wisc.edu
// CS Login:         zsong, dongh
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include "range_tree.h"




//initialize note
Node* init_node(void* this_addr, size_t this_len) {
    Node* this_node = calloc(1, sizeof(Node));
    this_node->addr = this_addr;
    this_node->len = this_len;
    this_node->freed = N_FREE;
    this_node->color = RED;
    this_node->stat = NODE;
    this_node->parent = NULL;
    this_node->left = NULL;
    this_node->right = NULL;
    this_node->err = 0; // 0 for no error, 1 for error
                        
    return this_node;
}

//initializa pointer to root
Node* init_tree() {
    Node* this_node = calloc(1, sizeof(Node));
    this_node->addr = NULL;
    this_node->len = NOTHING;
    this_node->freed = NOTHING;
    this_node->color = NOTHING;
    this_node->stat = ROOT;
    this_node->parent = NULL;
    this_node->left = NULL;
    this_node->right = NULL;
    this_node->err = 0;
    return this_node;
}

//initialize leaf
Node* init_leaf(Node* parent) {
    Node* this_node = calloc(1, sizeof(Node));
    this_node->addr = NULL;
    this_node->len = NOTHING;
    this_node->freed = NOTHING;
    this_node->color = BLACK;
    this_node->stat = LEAF;
    this_node->parent = parent;
    this_node->left = NULL;
    this_node->right = NULL;
    this_node->err = 0;
    return this_node;
}

//initialize RBT
RBT* init_RBT() {
    RBT* this_RBT = calloc(1, sizeof(RBT));
    this_RBT->tree = init_tree();
	this_RBT->tree->left = init_leaf(this_RBT->tree);
	this_RBT->tree->right = init_leaf(this_RBT->tree);
    return this_RBT;
}




void print_level(Node* root, int level) {
    if (root->stat == LEAF) {
        if (level == 1)
            printf("LEAF ");
        return;

    }
    if (level == 1) {
        if (root->color == RED)
            printf("(RED)%p ", root->addr);
        if (root->color == BLACK)
            printf("(BLACK)%p ", root->addr);
    }
    else if (level > 1)
    {
        print_level(root->left, level - 1);
        print_level(root->right, level - 1);
        printf("$$ ");
    }
}

void print_RBT(Node* root) {
    for (int i = 1; i <= 6; i++) {
        print_level(root, i);
        printf("\n");
    }
}





Node* find_parent(Node* node) {
    // Parent for a root is NULL
    return node->parent;
}

Node* find_grandparent(Node* node) {
    Node* parent = find_parent(node);
    if (parent->stat != ROOT) {
        return find_parent(parent);
    }
    return NULL;
}

Node* find_sibling(Node* node) {
    Node* parent = find_parent(node);
    if (parent->stat == ROOT) {
        return NULL;
    }
    else if (node == parent->right) {
        return parent->left;
    }
    else {
        return parent->right;
    }
}

Node* find_sibling_of_parent(Node* node) {
    Node* parent = find_parent(node);
    Node* grandparent = find_grandparent(node);
    if (grandparent->stat != ROOT) {
        //printf("Sibling of parent is %p\n",find_sibling(parent));
        return find_sibling(parent);
    }
    return NULL;
}

void left_rotate(Node* node) {
    Node* parent = find_parent(node);
    Node* newnode = node->right;
    // assert that the right child is not a leaf
    assert(newnode->stat != LEAF);

    node->right = newnode->left;
    newnode->left = node;
    node->parent = newnode;

    node->right->parent = node;

    // if node isn't a root initially
    if (node == parent->left) {
        parent->left = newnode;
    }
    else if (node == parent->right) {
        parent->right = newnode;
    }
    newnode->parent = parent;
}

void right_rotate(Node* node) {
    Node* parent = find_parent(node);
    Node* newnode = node->left;
    // assert that the right child is not a leaf
    assert(newnode->stat != LEAF);

    node->left = newnode->right;
    newnode->right = node;
    node->parent = newnode;

    node->left->parent = node;
    // if node isn't a root initially
    if (node == parent->left) {
        parent->left = newnode;
    }
    else if (node == parent->right) {
        parent->right = newnode;
    }
    newnode->parent = parent;
}

// helper method
void recursive_insert(Node* root, Node* node, void* this_addr, size_t this_len) {
    // Recursively track the tree down until a leaf is found

    if (root->stat != ROOT && node->addr < root->addr) {
        // See if root's left child is a leaf(null node)
        if (root->left->stat == LEAF) {
            root->left = node;
        }
        else {
            recursive_insert(root->left, node, this_addr, this_len);
            return;
        }
    }
    else if (root->stat != ROOT) {
        // See if root's right child is a leaf(null node)
        if (root->right->stat == LEAF) {
            root->right = node;
        }
        else {
            recursive_insert(root->right, node, this_addr, this_len);

            return;
        }
    }

    // if we successfully find the place we want to insert,
    // insert the new node
    node->left = init_leaf(node); // make it a leaf
    node->right = init_leaf(node); // make it a leaf
    node->parent = root;
    node->color = RED;
    node->stat = NODE;
    node->addr = this_addr;
    node->len = this_len;

}

// helper method
void repair_tree(Node* node) {
    // Repair tree by cases
    if (find_parent(node)->stat == ROOT) {
        // If the node is the root of the tree
        node->color = BLACK;
    }
    else if (find_parent(node)->color == BLACK) {
        // if the parent of the node is black
        // doesn't violate any red-black property
    }
    else if (find_sibling_of_parent(node)->color == RED) {
        // if the sibling of parent is red (and the parent is red)
        find_grandparent(node)->color = RED;
        find_parent(node)->color = BLACK;
        find_sibling_of_parent(node)->color = BLACK;
        // fix the problem caused by a RED grandparent
        // by using tail-recursive call
        repair_tree(find_grandparent(node));
    }
    else {
        // if the sibling of parent is black or NULL (and the parent is red)
        Node* grandparent = find_grandparent(node);
        Node* parent = find_parent(node);

        if (node == grandparent->left->right) {
            left_rotate(parent);
            node = node->left;
        }
        else if (node == grandparent->right->left) {
            right_rotate(parent);
            node = node->right;
        }

        if (node == parent->left) {
            right_rotate(grandparent);
        }
        else {
            left_rotate(grandparent);
        }
        grandparent->color = RED;
        parent->color = BLACK;

    }

}


void insert_node(RBT* t, Node* node) {

    //insert first node
    if (t->tree->left->stat == LEAF) {
        t->tree->left = node;
        node->left = init_leaf(node);
        node->right = init_leaf(node);
        node->parent = t->tree;
        node->color = BLACK;
    }
    //insert rest nodes
    else {
        Node* root = t->tree->left;
        // 1. Insert a new node into the current tree
        recursive_insert(root, node, node->addr, node->len);
        // 2. Repair the red black tree whenever
        // the red-black properties have been violated
        repair_tree(node);
        // 3. Return the new root and replace the previous one
        root = node;
        while (find_parent(root)->stat != ROOT) {
            root = find_parent(root);
        }

        t->tree->left = root;

    }

}




void delete_w_child_fix(Node* child) {

    //when the child is root (does not have parent)
    //recusion fix end at root
    if (child->parent->stat == ROOT) {
        return;
    }
    //when the child has parent
    else {
        //find child's siblin
        Node* sibling = NULL;
        if (child->parent->right == child)
            sibling = child->parent->left;
        else
            sibling = child->parent->right;

        //case1: parent = balck, child = black, sibling = red
        if (sibling->color == RED) {
            sibling->color = BLACK;
            child->parent->color = RED;
            if (child == child->parent->left)
                left_rotate(child->parent);
            else
                right_rotate(child->parent);
        }
        //case 2: parent = black, child = black, sibling = black, nephews = black, RECURSION FIX PARENT
        else if (child->parent->color == BLACK && sibling->color == BLACK &&
            sibling->left->color == BLACK && sibling->right->color == BLACK) {
            sibling->color = RED;
            delete_w_child_fix(child->parent);
        }
        //case 3: parent = red, child = black, sibling = black, nephews == black
        else if (child->parent->color == RED && sibling->color == BLACK &&
            sibling->left->color == BLACK && sibling->right->color == BLACK) {
            child->parent->color = BLACK;
            sibling->color = RED;

        }
        //case 4: child = parent left; sibling = black, left nephew == red, right nephew == black
        else if (child->parent->left == child && sibling->color == BLACK &&
            sibling->left->color == RED && sibling->right->color == BLACK) {
            sibling->left->color = BLACK;
            sibling->color = RED;
            right_rotate(sibling);
        }
        //case 5: child = parent left; sibling = black, right nephew == red, left nephew == black
        else if (child->parent->right == child && sibling->color == BLACK &&
            sibling->right->color == RED && sibling->left->color == BLACK) {
            sibling->right->color = BLACK;
            sibling->color = RED;
            left_rotate(sibling);
        }
        //case 6: sibling = black, one nephew == red, right nephew == black
        else {
            //int color_temp = sibling->color;
            sibling->color = child->parent->color;
            child->parent->color = BLACK;//color_temp;
                                         //child = parent left
            if (child->parent->right == child) {
                sibling->left->color = BLACK;
                right_rotate(child->parent);
            }
            //child = parent right
            else {
                sibling->right->color = BLACK;
                left_rotate(child->parent);
            }
        }

    }

}



void delete_w_child(Node* replacement_n, Node* n) {

    //find n's child
    Node* n_child = NULL;
    if (n->right->stat == LEAF)
        n_child = n->left;
    else
        n_child = n->right;

    if ((n->color == BLACK && n->left->stat == LEAF && n->right->stat == LEAF)) {
        delete_w_child_fix(n);
    }


    //replace n with n's child
    n_child->parent = n->parent;
    if (n->parent->left == n)
        n->parent->left = n_child;
    else
        n->parent->right = n_child;


    //if n = red deletion is complete !!!
    if (n->color == RED) {//leaf: ok; one child: ?

    }
    //if n = black
    else {
        //case 1: n's child = red 
        if (n_child->color == RED)
            n_child->color = BLACK;
        //case 2: n's child = black
        else {
            delete_w_child_fix(n_child);
        }
    }

    //free after delete
    if (replacement_n == NULL) {
        if (n->right->stat == LEAF) {
            free(n->right);
            n->left = NULL;
        }
        else {
            free(n->left);
            n->right = NULL;
        }
        free(n);
    }
    else {
        n->parent = replacement_n->parent;
        if (replacement_n->parent->left == replacement_n)
            replacement_n->parent->left = n;
        else
            replacement_n->parent->right = n;
        n->left = replacement_n->left;
        replacement_n->left->parent = n;
        n->right = replacement_n->right;
        replacement_n->right->parent = n;
        n->color = replacement_n->color;
        n->stat = replacement_n->stat;

        free(replacement_n);
    }
    
}



Node* find_largest(Node* n) {
    //if the node is smallest (most left) return
    if (n->right->stat == LEAF)
        return n;
    //if we have a smaller node (left child) recursivly find smallest node on the smaller node
    else
        return find_largest(n->right);
}




void delete_node(Node* this_n) {

    //if this node has 0 non-leaf child, delete it
    if (this_n->right->stat == LEAF || this_n->left->stat == LEAF)//ok
        delete_w_child(NULL, this_n);//LEAF

    else {
        //find next large node
        Node* next_small_n = find_largest(this_n->left);
        //when the replacement node has 0 non-leaf child
        delete_w_child(this_n, next_small_n);
    }
}

Node* search_single_node(Node* root, void* addr) {
    // traverse through all the nodes to find match
    Node* node = NULL;
    if (root->stat == ROOT) {
        return NULL;
    }

    // Base Case
    if (root->stat == LEAF) {
        return NULL;
    }

    // Recursive Case
    if (addr == root->addr) {
        // Mark the node free!!!
        root->err = 0;
        return root;
    }
    else if (addr > root->addr) {
        // Find if the address we want to find is in the middle of an allocated memory range
        if (addr > root->addr && addr < (root->addr + root->len)) {
            // Freeing memory that is not the first byte of the range of memory that was allocated.
            root->err = 1;
            return root;
        }
        else {
            // addr > root->addr and the addr is not in the middle of an allocated memory range, go for right child
            node = search_single_node(root->right, addr);
            return node;
        }
    }
    else {
        // addr < root->addr, go for left child
        node = search_single_node(root->left, addr);
        return node;
    }
}
//init list node
List_Node* init_list_node(Node* this_node) {
    List_Node* list_node = calloc(1, sizeof(List_Node));
    list_node->predecessor = NULL;
    list_node->successor = NULL;
    list_node->this_node = this_node;
    return list_node;
}
//init list
List* init_list() {
    List* list = calloc(1, sizeof(List));
    list->count = 0;
    list->start = init_list_node(init_tree());
    list->end = list->start;
    return list;
}
//add to delete list
void list_add(List* l, Node* node) {
    List_Node* n = init_list_node(node);
    l->end->successor = n;
    n->predecessor = l->end;
    l->end = n;
    l->count++;
}
//delete list
void list_op_delete(List* l) {
    List_Node* ptr = l->start->successor;
    for (int i = 0; i < l->count; i++) {
        delete_node(ptr->this_node);
        ptr = ptr->successor;
    }
}

void range_find_within_node(Node* root, void* addr, size_t len, List* delete_list) {
    if (root->stat == LEAF) {
        return;
    }
    else {
        //if current node is smaller than range
        if (root->addr < addr) {
            range_find_within_node(root->right, addr, len, delete_list);
        }
        //if current node is larger than range
        else if (root->addr > addr + len - 1) {
            range_find_within_node(root->left, addr, len, delete_list);
        }
        //if current node is within range
        else {
            if (root->freed == FREE)
                list_add(delete_list, root);
            else {
                printf("ERROR: malloc covered not freed area\n");
            }
            range_find_within_node(root->left, addr, root->addr - addr, delete_list);
            range_find_within_node(root->right, root->addr, addr + len - root->addr, delete_list);
        }

    }

}


void range_delete(RBT* t, void* addr, size_t len) {
    if (t->tree->left == NULL) {
        return;
    }
    else{
        List* delete_list = init_list();
        Node* first_n = search_single_node(t->tree->left, addr);
        //case: find first node
        if (first_n != NULL) {
            if (first_n->err == 1) {
                first_n->len = addr - first_n->addr;
                first_n->err = 0;
                if ((first_n->addr + first_n->len) < (addr + len)) {
                    free(delete_list);
                    return;
                }
            }
        }
        //range find node and add to list
        range_find_within_node(t->tree->left, addr, len, delete_list);
        //delete node in list
        list_op_delete(delete_list);

        free(delete_list);
    }
}
















void mem_range_check(Node* root, void* addr, size_t len) {
    
    // Base case
    if (root->stat == LEAF) {
        fprintf(stderr, "Error: The address range %p--%p specified has not been allocated by malloc537().\n", addr, (addr + len));
        exit(-1);
        return;
    }

    // Recursive case
    if (addr == root->addr) {
        if ((addr + len) > (root->addr + root->len)) {
            
            fprintf(stderr, "Error: The address range %p--%p specified falls in more than one region.\n", addr, (addr+len));
            exit(-1);
        }
        else {
            if (root->freed == 1) {
                
                fprintf(stderr, "Error: The address range %p--%p specified has been allocated by malloc537() but already freed by free537().\n", addr, (addr + len));
                exit(-1);
            }
        }
    }
    else if (addr > root->addr) {
        //printf("cccc\n");
        if (addr > root->addr && addr < (root->addr + root->len)) {
            if ((addr + len) > (root->addr + root->len)) {
                fprintf(stderr, "Error: The address range %p--%p specified falls in more than one region.\n", addr, (addr + len));
                exit(-1);
            }
            else {
                if (root->freed == 1) {
                    fprintf(stderr, "Error: The address range %p--%p specified has been allocated by malloc537() but already freed by free537().\n", addr, (addr + len));
                    exit(-1);
                }
            }
        }
        else {
            mem_range_check(root->right, addr, len);
            return;
        }
    }
    else {
        mem_range_check(root->left, addr, len);
        return;
    }

}













