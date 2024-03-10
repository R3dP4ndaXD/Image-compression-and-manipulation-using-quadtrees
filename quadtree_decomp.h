/*IACOB Andrei - 313CB*/
#ifndef QUADTREE_DECOMP_H
#define QUADTREE_DECOMP_H

#include<stdio.h>
#include<stdlib.h>
#include"queue.h"
#include"quadtree_struct.h"

void free_matr(unsigned char*** matr, unsigned int n);
void free_tree(QTree T);

QTree read_tree(char *file, unsigned int *n);
void decompress(QTree T, unsigned char **matr, unsigned int x, unsigned int y, unsigned int size);
void Task3(char* file1, char* file2);

#endif