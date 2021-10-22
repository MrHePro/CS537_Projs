////////////////////////////////////////////////////////////////////////////////
// Main File:        main.c
// This File:        graph_node.c
// Other Files:      main.c, makefile, graph_builder.c, graph_builder.h,
// 					 README, graph_node.h, partner.txt, proc_creator.c, proc_creator.h
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

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#define BUFFERSIZE 1024
#include "graph_node.h"


// initializa node
Node* create_node(int ref, int argc, char** target, Line** cmds, int line_number){
	Node* this_node = malloc(sizeof(Node));
	this_node -> ref_int = ref;
	this_node -> visited = 0;
	this_node -> list_size = 0;
	this_node -> adj_list = malloc(sizeof(int)*BUFFERSIZE);
	this_node -> node_argc = argc;
	this_node -> node_target = target;
	this_node -> run_flag = 0;
	this_node -> node_cmds = cmds;
	this_node -> node_line_number = line_number;
	return this_node;
}

void free_node(Node* this_node) {
    free(this_node -> adj_list);
    free(this_node);
    printf("Success: this_node -> adj_list freed.\n");
}

//print node for test
void print_node(Node* node){
	//printf("Note: ref: %d, visited: %d, list_size: %d, node_argc: %d, node: %s, run_flag: %d, ", node -> ref_int, node -> visited, 
    //node -> list_size, node -> node_argc, node -> node_target[0], node -> run_flag); 
	//printf("List: ");
	for(int i = 0; i < node -> list_size; i++ ){
		printf("%d，", (node -> adj_list)[i]);
	}
	//printf("\n");
	//printf("%d\n",node->node_argc);
	for (int z = 0; z < node->node_argc; z++) {
            printf("%s, ", node->node_target[z]);
    }
    //printf("\n\n");

}
