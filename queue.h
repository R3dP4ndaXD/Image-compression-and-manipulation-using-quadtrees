/*IACOB Andrei - 313CB*/
#ifndef QUEUE_H
#define QUEUE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"quadtree_struct.h"

typedef struct Celula {
    void* info;                         
    struct Celula *urm;
} TCelula, *TLista;               //lista generica  

typedef struct{
    TLista inc, sf;
} TCoada;

TCoada* InitQ();
int IntrQ(TCoada *c, void *elem);
int ExtQ(TCoada *c, void *elem, size_t size);
void DistrugeQ(TCoada **c);


typedef struct Celula_int {
    int info;
    struct Celula_int *urm;
} TCelula_int, *TLista_int;

typedef struct{
    TLista_int inc, sf;
} TCoada_int;

TCoada_int* InitQ_int();
int IntrQ_int(TCoada_int *c, int elem);
int ExtQ_int(TCoada_int *c, int *elem);
void DistrugeQ_int(TCoada_int **c);


#endif