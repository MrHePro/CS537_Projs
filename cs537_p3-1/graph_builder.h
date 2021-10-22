////////////////////////////////////////////////////////////////////////////////
// Main File:        main.c
// This File:        graph_builder.h
// Other Files:      main.c, makefile, graph_builder.c, README,
// 					 graph_node.c, graph_node.h, partner.txt, proc_creator.c, proc_creator.h
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

#ifndef graph_builder_h
#define graph_builder_h

#include "graph_node.h"
#include "text_parser.h"

typedef struct{
	int graph_size, traversed_size;
	Paragraph** graph_paras;
	Node** nodes;
	Node** traversed;
}Graph;

void free_graph(Graph* this_graph);

Graph* create_graph(Paragraph** paras);

Graph* add_node(Graph* this_graph, int argc, char** target, Line** cmds, int line_number);

Graph* create_adj(Graph* this_graph);

Graph* traverse(Graph* this_graph);

Graph* traverse_node(Graph* this_graph, char* this_node_name);

int visit(int node_ref, Node** all_nodes, Graph* this_graph, int* stack, int* stack_size);

int visit_node(int node_ref, Node** all_nodes, Graph* this_graph);


#endif