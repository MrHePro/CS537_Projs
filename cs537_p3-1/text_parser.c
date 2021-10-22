////////////////////////////////////////////////////////////////////////////////
// Main File:        main.c
// This File:        text_parser.c
// Other Files:      main.c, makefile, graph_builder.c, graph_builder.h,
//                   graph_node.c, graph_node.h, partner.txt, proc_creator.c, proc_creator.h
//                   README, text_parser.h
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
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include "text_parser.h"

#define BUFFSIZE 1024
#define LINESIZE 1024
#define PARAGRAPHSIZE 1024
#define ROWSIZE 1024
#define COLSIZE 1024

char* get_current_dir_name(void);

// Function to initialize a paragraph struct
Paragraph* create_para(){
    Paragraph* this_para= malloc(sizeof(Paragraph));
    this_para -> lines = malloc(sizeof(Line*) * LINESIZE);
    for(int i = 0; i < LINESIZE; i++){
        this_para -> lines[i] = NULL;
    }
    this_para->psize = 0;
    this_para->line_cnt = 0;
    return this_para;
}

void free_paras(Paragraph** this_paras) {
    int size = this_paras[0]->psize;
    for (int i = 0; i <= size ; i++) {
        for (int j = 0; j <= this_paras[i]->line_cnt; j++) {
            free_line(this_paras[i]->lines[j]);
        }
        free(this_paras[i]->lines);
        free(this_paras[i]);
    }
    free(this_paras);
}

// Function to initialize a line struct
Line* create_line(){
    Line* this_line = malloc(sizeof(Line));
    this_line -> type = -1;
    this_line -> argc = -1;
    this_line -> eflag = 0; // flag for error
    this_line -> eofflag = 0; // flag that signals for EOF
    this_line->line = NULL;
    this_line -> argv = malloc(sizeof(char*) * ROWSIZE);
    
    for(int i = 0; i < ROWSIZE; i++){
        this_line -> argv[i] = NULL;
    }
    
    return this_line;
}

void free_line(Line* this_line) {
    free(this_line->line);
    free(this_line->argv);
    free(this_line);
}

// Parse the makefile
Paragraph** text_parser(int other_file, char* file_name) {

    //OPEN FILE
    FILE* fp;
    
    // makefile/Makefile file names
    char* name1 = "makefile";
    char* name2 = "Makefile";
    
    //open files

    // if we have '-f' option
    if(other_file){

        if((fp = fopen(file_name, "r")) != NULL){

        }
        else{
            fprintf(stderr,"ERROR: -f file not found\n");
            fclose(fp);
            exit(0);
        }    
    }
    // if we don't have '-f' option
    else{

        // open makefile
        if ((fp = fopen(name1, "r")) != NULL) {
        }
        else {
            if ((fp = fopen(name2, "r")) != NULL) {
            }
            else {
                fprintf(stderr, "Error: No makefile/Makefile file found.\n");
                fclose(fp);
                exit(0);
            }
        }
    }

    //READ FILE

    int i = 0;// reader_buffer index
    int p_index = -1;// paragraph ondex
    int l_index = 0;
    int line_num = 1;
    int argv_index = 0;// argv index
    char * token1 = NULL;// malloc(BUFFSIZE*sizeof(char));
    char * token2 = NULL;//malloc(BUFFSIZE*sizeof(char));
    const char s2[2] = " ";// delimiter

    Paragraph** paragraphs = malloc(sizeof(Paragraph*) * PARAGRAPHSIZE);
    /*
    for(int p = 0; p < PARAGRAPHSIZE; p++){
        paragraphs[p] = NULL;
    }
    */
   
    // initialize the first paragraph
    //paragraphs[0] = create_para();
    //paragraphs[0] -> lines [0] = create_line();
    char *reader_buffer = malloc(BUFFSIZE * sizeof(char));

    // read until the end of the file
    while((reader_buffer[i] =fgetc(fp)) != EOF){
      
            if (i == 0 && reader_buffer[i] == '\t') {
                // when we get a command line
                paragraphs[p_index]->lines[l_index]->type = 1;
                
                i++;
                // ignore the first tab in commmand line
                i--;
                // read until the end of the line
                while ((reader_buffer[i] = fgetc(fp)) != '\n') {
                    if (i == BUFFSIZE-1) {
                        fprintf(stderr, "%d: Invalid line: %s\n", line_num, reader_buffer);
                        free(reader_buffer);
                        free_paras(paragraphs);
                        fclose(fp);
                        exit(0);
                    }
                

                    // we got a tab in the middle of a command line
                    if (i != 0 && (reader_buffer[i] == '\t' || reader_buffer[i] == ':')) {
                        paragraphs[p_index]->lines[l_index]->eflag = 1;
                        i++;
                        while ((reader_buffer[i] = fgetc(fp)) != '\n') {
                            i++;
                        }
                        reader_buffer[i] = '\0' ;
                        fprintf(stderr, "%d: Invalid line: %s\n", line_num, reader_buffer);
                        free(reader_buffer);
                        free_paras(paragraphs);
                        fclose(fp);
                        exit(0);
                    }
                    // we got a EOF in the end of a line
                    else if (i != 0 && (reader_buffer[i] == EOF )) {
                        paragraphs[p_index]->lines[l_index]->eofflag = 1;
                    }
                    // we got a comment in the middle of command line
                    else if(reader_buffer[i] == '#') {
                        paragraphs[p_index]->lines[l_index]->eflag = 1;
                        i++;
                        while ((reader_buffer[i] = fgetc(fp)) != '\n') {
                            if (i == BUFFSIZE - 1 || reader_buffer[i] == '\0') {
                                fprintf(stderr, "%d: Invalid line: %s\n", line_num, reader_buffer);
                                free(reader_buffer);
                                free_paras(paragraphs);
                                fclose(fp);
                                exit(0);
                            }
                            i++;
                        }
                        
                        break;
                    }
                    
                    if (paragraphs[p_index]->lines[l_index]->eofflag == 1) {
                        break;
                    }
                    i++;
                }
                reader_buffer[i] = '\0';
                if (paragraphs[p_index]->lines[l_index]->eflag == 0) {
                    if (i == 0) {
                        // this line is an empty line
                        fprintf(stderr, "%d: Invalid line: \n", line_num);
                        free(reader_buffer);
                        free_paras(paragraphs);
                        fclose(fp);
                        exit(0);
                        l_index--;
                    }
                    else {
                        // spilit the line by spaces
                        paragraphs[p_index] -> lines[l_index] -> line = reader_buffer;
                        token1 = strtok(reader_buffer, s2);
                        while (token1 != NULL) {
                            if (strcmp(token1, "\n") != 0) {
                                paragraphs[p_index]->lines[l_index]->argv[argv_index] = token1;
                                paragraphs[p_index]->lines[l_index]->argc++;
                                argv_index++;
                            }

                            token1 = strtok(NULL, s2);
                        }
                    }
                    
                    l_index++;
                    paragraphs[p_index]->line_cnt++;

                }
                
                if (paragraphs[p_index]->lines[l_index-1]->eofflag == 1) {
                    break;
                }
                

                i = 0;
                line_num++;
                argv_index = 0;
                reader_buffer = malloc(BUFFSIZE * sizeof(char));
                paragraphs[p_index]->lines[l_index] = create_line();
            }
            else if (i == 0 && reader_buffer[i] == '\n') {
                // when we get an empty newline
                i = 0;
                line_num ++;

            }
            else if (i == 0 && (reader_buffer[i] == ' ' || reader_buffer[i] == ':')) {
                // we got a spacce or colon at the start of a line
                while ((reader_buffer[i] = fgetc(fp)) != '\n') {
                        i++;
                }
                reader_buffer[i] = '\0';
                fprintf(stderr, "%d: Invalid line: %s\n", line_num, reader_buffer);
                free(reader_buffer);
                free_paras(paragraphs);
                fclose(fp);
                // exit for error
                exit(0);

            }
            else if (i == 0 && reader_buffer[i] == '#') {
                // when we get a comment line
                i++;
                while((reader_buffer[i] = fgetc(fp)) != '\n'){
                    // if we get a buffer overflow or a null character
                    if (i == BUFFSIZE - 1 || reader_buffer[i] == '\0') {
                        fprintf(stderr, "%d: Invalid line: %s\n", line_num, reader_buffer);
                        free(reader_buffer);
                        free_paras(paragraphs);
                        fclose(fp);
                        exit(0);
                    }
                    i++;
                }
                
            }

            else if(i == 0){
                // when we get a specification line
                p_index++;
                l_index = 0;

                paragraphs[p_index] = create_para();
                paragraphs[p_index] -> lines[l_index] = create_line();
                paragraphs[p_index] -> lines[l_index] -> type = 0;

                i++;
                // read until the end of the line
                while ((reader_buffer[i] = fgetc(fp)) != '\n') {
                    if (i == BUFFSIZE - 2) {
                        reader_buffer[i + 1] = '\0';
                        fprintf(stderr, "%d: Invalid line: %s\n", line_num, reader_buffer);
                        free(reader_buffer);
                        free_paras(paragraphs);
                        exit(0);
                    }
                    // we got a tab in the middle of a spec line
                    if (i != 1 && (reader_buffer[i] == '\t' )) {
                        paragraphs[p_index]->lines[l_index]->eflag = 1;
                        
                        while ((reader_buffer[i] = fgetc(fp)) != '\n') {
                            i++;
                        }
                        reader_buffer[i] = '\0';
                        fprintf(stderr, "%d: Invalid line: %s\n", line_num, reader_buffer);
                        free(reader_buffer);
                        free_paras(paragraphs);
                        exit(0);
                    }
                    // replace ':' by ' '
                    if (reader_buffer[i] == ':') {
                        reader_buffer[i] = ' ';
                    }
                    i++;
                }
                reader_buffer[i] = '\0';

                if (paragraphs[p_index]->lines[l_index]->eflag == 0) {
                    // spilit the line by spaces 
                    paragraphs[p_index] -> lines[l_index] -> line = reader_buffer;
                    token2 = strtok(reader_buffer, s2);
                    while (token2 != NULL) {
                        if (strcmp(token2, "\n") != 0) {
                            paragraphs[p_index]->lines[l_index]->argv[argv_index] = token2;
                            paragraphs[p_index]->lines[l_index]->argc++;
                            argv_index++;
                        }
                        token2 = strtok(NULL, s2);
                    }
                    
                    l_index++;
                    paragraphs[p_index]->line_cnt++;
                }
                
                i = 0;
                argv_index = 0;
                
                line_num++;
                reader_buffer = malloc(BUFFSIZE * sizeof(char));

                paragraphs[p_index]->lines[l_index] = create_line();
            }
            else{
                i++;
            }

            if (paragraphs[p_index]->lines[l_index - 1]->eofflag == 1) {
                break;
            }
    }
    paragraphs[0]->psize = p_index;
    free(reader_buffer);
    fclose(fp);

    return paragraphs;
}