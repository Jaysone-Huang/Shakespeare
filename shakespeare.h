#pragma once

#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    char *word;  // the name
    long counter; //counter
    struct Node *next;
} Node;

typedef struct LinkedList {
    // ll
    Node **ll;
    // index n size
    int n;
} LinkedList;
