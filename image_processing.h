#include"quadtree_decomp.h"
#include"quadtree_comp.h"
#include<time.h>
#include<math.h>
#include"queue.h"
#define MAX 255
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

typedef struct {
    int lin, col;
} coord;

void swap(QTree *T1, QTree *T2);
void flip_orizontal(QTree T);
void flip_vertical(QTree T);
void flip_dreapta(QTree T);
void flip_stanga(QTree T);
void gray_scaling(QTree T);
void negativ(QTree T);
void sepia(QTree T);
QTree overlap(QTree T1, QTree T2);
QTree overlay(QTree T1, QTree T2);
QTree intersect(QTree T1, QTree T2);
unsigned char** ConnectedComponentLabelling(unsigned char **matr, unsigned int size, unsigned int *nr_labels);
coord* Aloc_coord(int lin, int col);
void minimizare(QTree T);
void Gaussian_blur(unsigned char **matr, unsigned int size, unsigned int radius);
void Gaussian_blur_iter(unsigned char **matr, unsigned int size, unsigned int radius, unsigned int iter);
void Box_blur(unsigned char **matr, unsigned int size);
void Box_blur_iter(unsigned char **matr, unsigned int size, unsigned int iter);