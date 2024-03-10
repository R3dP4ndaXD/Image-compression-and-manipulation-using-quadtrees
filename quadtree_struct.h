/*IACOB Andrei - 313CB*/
#ifndef QUADTREE_STRUCT_H
#define QUADTREE_STRUCT_H

#include<stdio.h>

typedef struct node {
    unsigned char R, G, B;
    struct node *q1, *q2, *q3, *q4;
} Node, *QTree;

#endif