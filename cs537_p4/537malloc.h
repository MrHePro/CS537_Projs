////////////////////////////////////////////////////////////////////////////////
// Main File:        N/A
// This File:        537malloc.h
// Other Files:      README, makefile, 537malloc.c,
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
#ifndef __537malloc_h
#define __537malloc_h

void* malloc537(size_t size);

void free537(void* ptr);

void* realloc537(void* ptr, size_t size);

void memcheck537(void* ptr, size_t size);

#endif