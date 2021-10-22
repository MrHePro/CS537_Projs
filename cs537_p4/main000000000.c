#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "range_tree.h"

int main() {
    // insert some address and length
    RBT* test_RBT = init_RBT();

    Node* my_node100 = init_node(100,10);
    insert_node(test_RBT,my_node100);

    Node* my_node200 = init_node(200,10);
    insert_node(test_RBT,my_node200);

    Node* my_node300 = init_node(300,10);
    insert_node(test_RBT,my_node300);

    Node* my_node400 = init_node(400,10);
    insert_node(test_RBT,my_node400);

    Node* my_node500 = init_node(500,10);
    insert_node(test_RBT,my_node500);

    Node* my_node600 = init_node(600,10);
    insert_node(test_RBT,my_node600);

    Node* my_node700 = init_node(700,10);
    insert_node(test_RBT,my_node700);

    //Node* my_node650 = init_node(650,10);
    //insert_node(test_RBT,my_node650);





    printf("\n");
    print_RBT(test_RBT->tree->left);

    //delete_node(my_node600);

    print_RBT(test_RBT->tree->left);

    // free 537 test
    
    Node* found0_node = search_single_node(test_RBT->tree->left, 500);
    // detect free error
    if (found0_node == NULL) {
        fprintf(stderr, "Error: Freeing memory that has not be allocated with malloc537().\n");
        exit(-1);
    }
    else {
        if (found0_node->err == 1) {
            fprintf(stderr, "Error: Freeing memory that is not the first byte of the range of memory that was allocated.\n");
            exit(-1);
        }
        else if (found0_node->freed == 1) {
            fprintf(stderr, "Error: Freeing memory that was previously freed(double free).\n");
            exit(-1);
        }
        else {
            //printf("aaaa\n");
            printf("We found the node with addr %d\n", found0_node->addr);
            found0_node->freed = 1;
        }
    }

    /*
    Node* found1_node = search_single_node(test_RBT->tree->left, 500);
    // detect free error
    if (found1_node == NULL) {
        fprintf(stderr, "Error: Freeing memory that has not be allocated with malloc537().\n");
        exit(-1);
    }
    else {
        if (found1_node->err == 1) {
            fprintf(stderr, "Error: Freeing memory that is not the first byte of the range of memory that was allocated.\n");
            exit(-1);
        }
        else if (found1_node->freed == 1) {
            fprintf(stderr, "Error: Freeing memory that was previously freed(double free).\n");
            exit(-1);
        }
        else {
            //printf("bbbb\n");
            printf("We found the node with addr %d\n", found1_node->addr);
            found1_node->freed = 1;
        }
    }
    */
    
    int addr = 508;
    int len = 3;

    mem_range_check(test_RBT->tree->left, addr, len);

    //print_RBT(test_RBT->tree->left);

    /*
    printf("Color: %d root of addr %d has a left child of addr %d and right child of addr %d\n", test_RBT->tree->left->color, test_RBT->tree->left->addr, test_RBT->tree->left->left->addr, test_RBT->tree->left->right->addr); //100
    printf("Color: %d node100 of addr %d has a left child of addr %d and right child of addr %d\n", my_node100->color, my_node100->addr, my_node100->left->addr, my_node100->right->addr); //200
    printf("Color: %d node200 of addr %d has a left child of addr %d and right child of addr %d\n", my_node200->color, my_node200->addr, my_node200->left->addr, my_node200->right->addr); //200
    printf("Color: %d node300 of addr %d has a left child of addr %d and right child of addr %d\n", my_node300->color, my_node300->addr, my_node300->left->addr, my_node300->right->addr); //300
    printf("Color: %d node400 of addr %d has a left child of addr %d and right child of addr %d\n", my_node400->color, my_node400->addr, my_node400->left->addr, my_node400->right->addr); //400
    printf("Color: %d node500 of addr %d has a left child of addr %d and right child of addr %d\n", my_node500->color, my_node500->addr, my_node500->left->addr, my_node500->right->addr); //500
    printf("Color: %d node600 of addr %d has a left child of addr %d and right child of addr %d\n", my_node600->color, my_node600->addr, my_node600->left->addr, my_node600->right->addr); //600
    printf("Color: %d node700 of addr %d has a left child of addr %d and right child of addr %d\n", my_node700->color, my_node700->addr, my_node700->left->addr, my_node700->right->addr); //700
	printf("Color: %d node650 of addr %d has a left child of addr %d and right child of addr %d\n", my_node650->color, my_node650->addr, my_node650->left->addr, my_node650->right->addr); //700
    */


}