////////////////////////////////////////////////////////////////////////////////
// Main File:        N/A
// This File:        range_tree.h
// Other Files:      README, makefile, 537malloc.h,
// 					 range_tree.c, 537malloc.c
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
#ifndef range_tree_h
#define range_tree_h

#define RED 0
#define BLACK 1
#define D_BLACK 2
#define ROOT 3
#define NODE 4
#define LEAF 5
#define NOTHING -1
#define FREE 1
#define N_FREE 0

typedef struct Node Node;

struct Node {
    void* addr;
    size_t len;
    int freed;
    int color;
    int stat;
    int err;
    //int allocated;
    Node* parent;
    Node* left;
    Node* right;
};

typedef struct {
    Node* tree;
}RBT;

typedef struct List_Node List_Node;

struct List_Node {
    List_Node* predecessor;
    List_Node* successor;
    Node* this_node;
};

typedef struct {
    int count;
    List_Node* start;
    List_Node* end;
}List;


Node* init_node(void* this_addr, size_t this_len);

RBT* init_RBT();

void print_level(Node* root, int level);

void print_RBT(Node* root);

void insert_node(RBT* t, Node* node);

Node* find_largest(Node* n);

void delete_node(Node* this_n);

Node* search_single_node(Node* root, void* addr);

void range_delete(RBT* t, void* addr, size_t len);

void mem_range_check(Node* root, void* addr, size_t len);

#endif