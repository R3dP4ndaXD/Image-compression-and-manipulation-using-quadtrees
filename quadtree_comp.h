/*IACOB Andrei - 313CB*/
#ifndef QUADTREE_COMP_H
#define QUADTREE_COMP_H

#include<stdio.h>
#include<stdlib.h>
#include"queue.h"
#include"quadtree_struct.h"

unsigned char** read_matr(char *file, unsigned int *n);
void free_matr(unsigned char*** matr, unsigned int n);
void means(unsigned char **matr, unsigned int x, unsigned int y, unsigned int size, unsigned char *R, unsigned char *G, unsigned char *B, unsigned int *score);
QTree compress(unsigned char **matr, unsigned int factor, unsigned int x, unsigned int y, unsigned int size);
void free_tree(QTree T);
int nr_levels(QTree T);
int nr_leaves(QTree T);
int first_leaf_level(QTree T);
int max_block(QTree T, unsigned int n);
void Task1(QTree T, unsigned int n, char* file);
void Task2(QTree T, unsigned int n, char* file);

#endif