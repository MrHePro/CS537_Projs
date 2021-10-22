////////////////////////////////////////////////////////////////////////////////
// Main File:        main.c
// This File:        proc_creator.c
// Other Files:      main.c, makefile, graph_builder.c, graph_builder.h,
// 					 graph_node.c, graph_node.h, partner.txt, README, proc_creator.h
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
#include <sys/wait.h>
#include "proc_creator.h"


void proc_creator(char** argv){
	int rc = fork();
	//fork failed; exit
	if (rc < 0){
		fprintf(stderr, "ERROR: forking process failed\n");
		exit(1);
	}
	else if (rc == 0){
		execvp(argv[0],argv);
		fprintf(stderr, "ERROR: executing failed\n");
	}
	else{
		wait(NULL);
	}

}