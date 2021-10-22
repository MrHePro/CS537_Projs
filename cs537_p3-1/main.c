////////////////////////////////////////////////////////////////////////////////
// Main File:        main.c
// This File:        main.c
// Other Files:      ERADME, makefile, graph_builder.c, graph_builder.h,
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

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "graph_builder.h"
#include "graph_node.h"
#include "text_parser.h"
#include "proc_creator.h"
#define BUFFERSIZE 1024
#define ROWSIZE 1024
#define COLSIZE 1024



long int getModtime(char* filename) {
    // get the most recent modification time of a file
    struct stat fileStats;
    if(stat(filename, & fileStats) == -1){
        fprintf(stderr, "Error: File not found when we get the modification time.\n");
        exit(0);
    }
    
    time_t mod_time = fileStats.st_mtime;
    // convert mod_time to int
    long int mtime = (long)(mod_time);
    return mtime;
}


int main(int argc, char** argv){
	Paragraph** paras; 
    
    
    char* target = NULL;//malloc(sizeof(char)*BUFFERSIZE);
    //printf("111\n");
	if(argc == 3 && strcmp(argv[1],"-f") == 0){
        // 537make -f filename
		paras = text_parser(1, argv[2]);

	} 
	else if(argc == 1){
        // 537make  
        //free(paras);
		paras = text_parser(0, argv[0]);
	}
    else if (argc == 2) {
        // 537make target
        paras = text_parser(0, argv[0]);
        //free(target);
        target = argv[1];

    }
    else if (argc == 4 && strcmp(argv[1], "-f") == 0){
        // 537make -f filename target
        paras = text_parser(1, argv[2]);
        //free(target);
        target = argv[3];

    }
	else{
		fprintf(stderr, "USAGE: 537make (-f <filename>) (<target>)\n");
	}
    //printf("222\n");
    //create graph
    Graph* graph = create_graph(paras);
    
    for (int i = 0; i <= paras[0] -> psize; i++) {
        add_node(graph, paras[i]->lines[0]->argc, paras[i]->lines[0]->argv, paras[i] -> lines, paras[i] -> line_cnt);
    }

    graph = create_adj(graph);

    graph = traverse(graph);

    if (argc == 2 || argc == 4) {
        traverse_node(graph, target);
    }

    //exe
    int argc_exe = graph -> traversed[0] -> node_argc;
    char** argv_exe = graph -> traversed[0] -> node_target;

    int create_flag = 0;
    int target_flag = 0;

    //for evry  spec need to run
    for(int i = 0; i < graph -> traversed_size; i++){

        argc_exe = graph -> traversed[i] -> node_argc;
        argv_exe = graph -> traversed[i] -> node_target;

        //assume we do not need to create the exe
        create_flag = 0;
        if(argc_exe == 1){
        	create_flag = 1;
        }
       	else{
       		//for each dependency
	        for(int j = 1; j < argc_exe; j++){

	            //assume it is not a target
	            target_flag = 0;
	            //if dependency is a target: set target flag
	            for(int k = 0; k < i; k++){
	               
	                if(strcmp(graph -> traversed[k] -> node_target[0], argv_exe[j]) == 0){
	                    target_flag = 1;
	                    //if dependency has run: set create flag
	                    if(graph -> traversed[k] -> run_flag == 1){

	                        create_flag = 1;
	                    }
	                    break;
	                }
	            }
	            //if dependency is a filename: check mod time to set create flag.
	            if(!target_flag){
	                struct stat fileStats;
    				if(stat(argv_exe[0], & fileStats) == -1){
        				create_flag = 1;
    				}else if(getModtime(argv_exe[0]) < getModtime(argv_exe[j])){
	                    create_flag = 1;
	                }
	            }
	        }
	    }
    	
        //if needed to exe, exe it and set run flag.
        if(create_flag){
            for(int c = 1; c < graph -> traversed[i] -> node_line_number; c++){
                proc_creator(graph -> traversed[i] -> node_cmds[c] -> argv);
            }
            graph -> traversed[i] -> run_flag = 1;
        } 

        
    }

    free_graph(graph);

    // free all dynamic allocations
    //free(target);
    




}
