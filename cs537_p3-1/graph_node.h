////////////////////////////////////////////////////////////////////////////////
// Main File:        main.c
// This File:        graph_node.h
// Other Files:      main.c, makefile, graph_builder.c, graph_builder.h,
// 					 graph_node.c, README, partner.txt, proc_creator.c, proc_creator.h
//                   text_parser.c, text_parser.h
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

#ifndef graph_node_h
#define graph_node_h

#include "text_parser.h"

typedef struct node{
	int ref_int, visited, list_size, node_argc, run_flag, node_line_number;
	int* adj_list;
	char** node_target;
	Line** node_cmds;
} Node;

void free_node(Node* this_node);
Node* create_node(int ref, int argc, char** target, Line** cmds, int line_number);
void print_node(Node* node);

#endif