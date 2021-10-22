////////////////////////////////////////////////////////////////////////////////
// Main File:        N/A
// This File:        537malloc.c
// Other Files:      README, makefile, 537malloc.h,
// 					 range_tree.c, range_tree.h
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
#include "range_tree.h"
#include "537malloc.h"

RBT* t = NULL;

void* malloc537(size_t size) {
    //In addition to actually allocating the memory by calling malloc(),
    //this function will record a tuple(addri, leni), for the memory that you allocate in the heap.
    //(If the allocated memory was previously freed, this will be a bit more complicated.)
    //You will get the starting address, addri, from the return value from malloc() and the length, leni, from the size parameter.
    //You can check the size parameter for zero length(this is not actually an error, but unusual enough that it is worth reporting).

    // if RBT doesn't exist, initialize RBT
    if(t == NULL){
        t = init_RBT();
    }

    void* addr = NULL;

    if (size == 0) {
        // warning: malloc of size 0
        printf("WARNING: Malloc size of 0.\n");
        return addr;
    }

    addr = (void*)malloc(size);

    // remove all the nodes in the range
    // consider edges case
    // delete existing free blocks that overlap
    range_delete(t, addr, size);

    // insert the new node
    insert_node(t,init_node(addr, size));

    return addr;

}

void free537(void* ptr) {
    void* addr = ptr;
    if (ptr == NULL) {
        fprintf(stderr, "Error: Freeing NULL pointer.\n");
        exit(-1);
    }
    Node* found_node;

    //Error Cases:  
    
    if ((found_node = search_single_node(t->tree->left,addr)) != NULL) {
        if (found_node->err == 1) {
            // ERROR1: Freeing memory that is not the first byte of the range of memory that was allocated.
            fprintf(stderr, "Error: Freeing memory that is not the first byte of the range of memory that was allocated.\n");
            exit(-1);
        }
        else if (found_node->freed == 1) {
            // ERROR2: Freeing memory that was previously freed(double free).
            // double free
            fprintf(stderr, "Error: Freeing memory that was previously freed(double free).\n");
            exit(-1);
        }
        else {
            // correct case, let's free
            // we've marked it free in the search function
            found_node->freed = 1;
            free(ptr);
        }
    }
    else {        
        // ERROR3: Freeing memory that has not be allocated with malloc537().
        fprintf(stderr, "Error: Freeing memory that has not be allocated with malloc537().\n");
        exit(0);
    }

}

void* realloc537(void* ptr, size_t size) {
    // if ptr is NULL, call malloc537()
    if (ptr == NULL) {
        ptr = malloc537(size);
        return ptr;
    }
    // if size = 0, call free537()
    else if(size == 0){
        printf("WARNING: Realloc size of 0.\n");
        free537(ptr);
        return NULL;
    }
    else {
        // In addition to changing the memory allocation by calling realloc()
        void* new_ptr = realloc(ptr, size);
        //first check to see if there was a tuple for the addr = ptr
        Node* rm_node = search_single_node(t->tree->left, ptr);
        if (rm_node == NULL) {
            // do nothing
        }
        else if (rm_node->err == 1) {
            fprintf(stderr, "Error: Freeing memory that is not the first byte of the range of memory that was allocated.\n");
            exit(-1);
        }
        else {
            // remove that tuple
            delete_node(rm_node);
        }

        // then adds a new one where addr is the return value from realloc() and len is size
        insert_node(t,init_node(new_ptr, size));

        return new_ptr;
    }

}

void memcheck537(void* ptr, size_t size) {
    //This function checks to see the address range specified by address ptr and length size
    //are fully within a range allocated by malloc537() and memory not yet freed by free537().
    //When an error is detected, then print out a detailed and informative error message and exit the program(with a - 1 status).
    void* addr = ptr;
    size_t len = size;

    // mem check and print error message
    mem_range_check(t->tree->left, addr, len);//
  

}