#include"image_processing.h"

void swap(QTree *T1, QTree *T2) {
    QTree aux = *T1;
    *T1 = *T2;
    *T2 = aux;
}

/*
    parcurg pana la frunze si la iesirea din recursivitate reordonez fii
*/

void flip_orizontal(QTree T) {
    if(!T->q1) {
        return;
    }
    
    flip_orizontal(T->q1);
    flip_orizontal(T->q2);
    flip_orizontal(T->q3);
    flip_orizontal(T->q4);
    swap(&T->q1, &T->q3);
    swap(&T->q2, &T->q4);
}

void flip_vertical(QTree T) {
    if(!T->q1) {
        return;
    }
    
    flip_vertical(T->q1);
    flip_vertical(T->q2);
    flip_vertical(T->q3);
    flip_vertical(T->q4);
    swap(&T->q1, &T->q2);
    swap(&T->q3, &T->q4);
}
void flip_dreapta(QTree T) {
    if(!T->q1) {
        return;
    }
    
    flip_dreapta(T->q1);
    flip_dreapta(T->q2);
    flip_dreapta(T->q3);
    flip_dreapta(T->q4);
    swap(&T->q1, &T->q2);
    swap(&T->q3, &T->q4);
    swap(&T->q1, &T->q3);
}

void flip_stanga(QTree T) {
    if(!T->q1) {
        return;
    }
    
    flip_stanga(T->q1);
    flip_stanga(T->q2);
    flip_stanga(T->q3);
    flip_stanga(T->q4);
    swap(&T->q1, &T->q4);
    swap(&T->q2, &T->q3);
    swap(&T->q1, &T->q3);
}

/*
    parcurg pana la frunze si la iesirea din recursivitate recalculez valorile campurilor R, G, B
*/
void gray_scaling(QTree T) {
    if(!T) {
        return;
    }
    gray_scaling(T->q1);
    gray_scaling(T->q2);
    gray_scaling(T->q3);
    gray_scaling(T->q4);
    unsigned char gray = (unsigned char)(0.299 * T->R + 0.587 * T->G + 0.114 * T->B);
    T->R = gray;
    T->G = gray;
    T->B = gray;
}

void negativ(QTree T) {
    if(!T) {
        return;
    }
    negativ(T->q1);
    negativ(T->q2);
    negativ(T->q3);
    negativ(T->q4);
    T->R = ~T->R;
    T->G = ~T->G;
    T->B = ~T->B;
}

void sepia(QTree T) {
    if(!T) {
        return;
    }
    sepia(T->q1);
    sepia(T->q2);
    sepia(T->q3);
    sepia(T->q4);
    unsigned int newR, newG, newB;
    newR = T->R * .393 + T->G * .769 + T->B * .189;
    newG = T->R * .349 + T->G * .686 + T->B * .168;
    newB = T->R * .272 + T->G * .534 + T->B * .131;
    T->R = (unsigned char)((newR < MAX) ? newR : MAX);
    T->G = (unsigned char)((newG < MAX) ? newG : MAX);
    T->B = (unsigned char)((newB < MAX) ? newB : MAX);
}

/*
    Parcurg arborii si costruiesc un nou arbore in care culorile din fiecare nod sunt date de media culorile a celor 2 arbori
*/
QTree overlap(QTree T1, QTree T2) {
    QTree T3 = calloc(1, sizeof(Node));
    if (!T3) {
        return NULL;
    }
    if (T1->q1 && T2->q1) {
        T3->q1 = overlap(T1->q1, T2->q1);
        T3->q2 = overlap(T1->q2, T2->q2);
        T3->q3 = overlap(T1->q3, T2->q3);
        T3->q4 = overlap(T1->q4, T2->q4);
    } else if (!T1->q1 && T2->q1) {
        T3->q1 = overlap(T1, T2->q1);
        T3->q2 = overlap(T1, T2->q2);
        T3->q3 = overlap(T1, T2->q3);
        T3->q4 = overlap(T1, T2->q4);
    } else if (T1->q1 && !T2->q1) {
        T3->q1 = overlap(T1->q1, T2);
        T3->q2 = overlap(T1->q2, T2);
        T3->q3 = overlap(T1->q3, T2);
        T3->q4 = overlap(T1->q4, T2);
    }
    T3->R = (T1->R + T2->R) / 2;
    T3->G = (T1->G + T2->G) / 2;
    T3->B = (T1->B + T2->B) / 2;
    return T3;
}

/*
    Parcurg arborii si costruiesc un nou arbore in care culorile din fiecare nod sunt date de operatia de sau logic a culorile a celor 2 arbori
*/
QTree overlay(QTree T1, QTree T2) {
    QTree T3 = calloc(1, sizeof(Node));
    if (!T3) {
        return NULL;
    }
    if (T1->q1 && T2->q1) {
        T3->q1 = overlay(T1->q1, T2->q1);
        T3->q2 = overlay(T1->q2, T2->q2);
        T3->q3 = overlay(T1->q3, T2->q3);
        T3->q4 = overlay(T1->q4, T2->q4);
    } else if (!T1->q1 && T2->q1) {
        T3->q1 = overlay(T1, T2->q1);
        T3->q2 = overlay(T1, T2->q2);
        T3->q3 = overlay(T1, T2->q3);
        T3->q4 = overlay(T1, T2->q4);
    } else if (T1->q1 && !T2->q1) {
        T3->q1 = overlay(T1->q1, T2);
        T3->q2 = overlay(T1->q2, T2);
        T3->q3 = overlay(T1->q3, T2);
        T3->q4 = overlay(T1->q4, T2);
    }
    T3->R = T1->R | T2->R;
    T3->G = T1->G | T2->G;
    T3->B = T1->B | T2->B;
    return T3;
}

/*
    Parcurg arborii si costruiesc un nou arbore in care culorile din fiecare nod sunt date de operatia de si logic a culorile a celor 2 arbori
*/
QTree intersect(QTree T1, QTree T2) {
    QTree T3 = calloc(1, sizeof(Node));
    if (!T3) {
        return NULL;
    }
    if (T1->q1 && T2->q1) {
        T3->q1 = intersect(T1->q1, T2->q1);
        T3->q2 = intersect(T1->q2, T2->q2);
        T3->q3 = intersect(T1->q3, T2->q3);
        T3->q4 = intersect(T1->q4, T2->q4);
    } else if (!T1->q1 && T2->q1) {
        T3->q1 = intersect(T1, T2->q1);
        T3->q2 = intersect(T1, T2->q2);
        T3->q3 = intersect(T1, T2->q3);
        T3->q4 = intersect(T1, T2->q4);
    } else if (T1->q1 && !T2->q1) {
        T3->q1 = intersect(T1->q1, T2);
        T3->q2 = intersect(T1->q2, T2);
        T3->q3 = intersect(T1->q3, T2);
        T3->q4 = intersect(T1->q4, T2);
    }
    T3->R = T1->R & T2->R;
    T3->G = T1->G & T2->G;
    T3->B = T1->B & T2->B;
    return T3;
}

/*
    sa parcurge pana la ultimele noduri interne si daca fii sunt identici, acestia se elibeaza si nodul tata devine frunza
    ia iesirea din recurisivitate, se aplica repetat acesta operatie DOAR nodurilor care ajung sa aiba toti fii frunze 
*/
void minimizare(QTree T) {
    if (!T->q1) {
        return;
    }
    minimizare(T->q1);
    minimizare(T->q2);
    minimizare(T->q3);
    minimizare(T->q4);
    if (!T->q1->q1 && !T->q2->q1 && !T->q3->q1 && !T->q4->q1 && T->q1->R == T->q2->R && T->q2->R == T->q3->R && T->q3->R == T->q4->R && T->q1->G == T->q2->G && T->q2->G == T->q3->G && T->q3->G == T->q4->G && T->q1->B == T->q2->B && T->q2->B == T->q3->B && T->q3->B == T->q4->B) {
        T->R = T->q1->R;
        T->G = T->q1->G;
        T->B = T->q1->B;
        free(T->q1);
        free(T->q2);
        free(T->q3);
        free(T->q4);
        T->q1 = NULL;
        T->q2 = NULL;
        T->q3 = NULL;
        T->q4 = NULL;
    }

}

/*
    Alocare structura de coordonate
*/
coord* Aloc_coord(int lin, int col) {
    coord *coo = malloc(sizeof(coord));
    if (!coo) {
        return NULL;
    }
    coo->lin = lin;
    coo->col = col;
    return coo;
}

unsigned char** ConnectedComponentLabelling(unsigned char **matr1, unsigned int size, unsigned int *nr_labels) {
    unsigned char label_R,label_G, label_B;
    int i, j, lin, col, count = 0;
    *nr_labels = 0;
    label_R = rand() % (MAX - 1) + 1;
    label_G = rand() % (MAX - 1) + 1;
    label_B = rand() % (MAX - 1) + 1;
    unsigned char **matr2 = calloc(size, sizeof(unsigned char *));
    if (!matr2) {
        return NULL;
    }
    for (i = 0; i < size; i++) {
        matr2[i] = calloc(3 * size, sizeof(unsigned char));
        if (!matr2[i]) {
            free_matr(&matr2, i);
            return NULL;
        }
    }
    TCoada *c;
    coord *coo = NULL;
    c = InitQ();
    if (!c) {
        free_matr(&matr2, size);
        return NULL;
    }
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (matr1[i][3 *j] == 0 && matr2[i][3 * j] == 0) {  //daca pixelul e negru si nu a fost etichetat
                coo = Aloc_coord(i, j);
                IntrQ(c, coo);
                while(c->inc) {
                    ExtQ(c, &coo, sizeof(coord *));     //scot un un set de coordonate din coada
                    lin = coo->lin;
                    col = coo->col;
                    free(coo);
                    //verifica daca nu ies din matrice si daca pixelul e negru si neetichetat
                    if (lin >= 0 && col < size && col >= 0 && lin < size && matr1[lin][3 * col] == 0 && matr2[lin][3 * col] == 0) {
                        matr2[lin][3 * col]=label_R;
                        matr2[lin][3 * col + 1]=label_G;
                        matr2[lin][3 * col + 2]=label_B;
                        coo = Aloc_coord(lin - 1, col);
                        IntrQ(c, coo);
                        coo = Aloc_coord(lin + 1, col);
                        IntrQ(c, coo);
                        coo = Aloc_coord(lin, col-1);
                        IntrQ(c, coo);
                        coo = Aloc_coord(lin, col + 1);
                        IntrQ(c, coo);
                    }
                }
                label_R = rand() % (MAX - 1) + 1;
                label_G = rand() % (MAX - 1) + 1;
                label_B = rand() % (MAX - 1) + 1;
                count++;
            }
        }
    }
    *nr_labels = count;
    free(c);
    c = NULL;
    coo = NULL;
    return matr2;
}


double** Gaussian_kernel(unsigned int radius) {
    double sigma = ((double)(radius / 2) > 1) ? radius / 2 : 1;
    double r, s = 2.0 * sigma * sigma;
    double sum = 0.0;
    register int x, y, i, j;
    unsigned int kernelWidth = (unsigned int)(2 * radius + 1);
    double **GKernel = calloc(kernelWidth, sizeof(double *));
    if (!GKernel) {
        return NULL;
    }
    for (i = 0; i < kernelWidth; i++) {
        GKernel[i] = calloc(kernelWidth, sizeof(double));
        if (!GKernel[i]) {
            for (j = 0; j < i; j++) {
                free(GKernel[i]);
            }
            free(GKernel);
            return NULL;
        }
    }
    for (x = -radius; x <= (int)radius; x++) {
        for (y = -radius; y <= (int)radius; y++) {
            r = sqrt(x * x + y * y);
            GKernel[x + radius][y + radius] = (exp(-(r * r) / s)) / (M_PI * s);
            sum += GKernel[x + radius][y + radius];
        }
    }
    for (x = 0; x < kernelWidth; x++) {
        for (y = 0; y < kernelWidth; y++) {
            GKernel[x][y] /= sum;
        }
    }
    return GKernel;
}

void Gaussian_blur(unsigned char **matr, unsigned int size, unsigned int radius) {
    clock_t start, stop;
    start = clock();
    double sumR, sumG, sumB;
    register int x, y, i, j;
    unsigned int kernelWidth = (2 * radius) + 1;
    double **kernel = Gaussian_kernel(radius);
    if(!kernel) {
        return;
    }
    for (x = 0; x < size; x++) {
        for (y = 0; y < size; y++) {
            sumR = 0;
            sumG = 0;
            sumB = 0;
            for (i = -radius; i <= (int)radius; i++) {
                for (j = -radius; j <= (int)radius; j++) {
                    if (x + i >= 0 && x + i < size && y + j >= 0 && y + j < size) {                          //interior
                        sumR += (double)kernel[i + radius][j + radius] * matr[x + i][3 * (y + j)];
                        sumG += (double)kernel[i + radius][j + radius] * matr[x + i][3 * (y + j) + 1];
                        sumB += (double)kernel[i + radius][j + radius] * matr[x + i][3 * (y + j) + 2];
                    } else if (x + i < 0 && y + j < 0) {                                                     //colt stanga sus
                        sumR += (double)kernel[i + radius][j + radius] * matr[0][0];
                        sumG += (double)kernel[i + radius][j + radius] * matr[0][1];
                        sumB += (double)kernel[i + radius][j + radius] * matr[0][2];
                    } else if (x + i < 0 && y + j >= size) {                                                 //colt dreapta sus
                        sumR += (double)kernel[i + radius][j + radius] * matr[0][3 * (size - 1)];
                        sumG += (double)kernel[i + radius][j + radius] * matr[0][3 * (size - 1) + 1];
                        sumB += (double)kernel[i + radius][j + radius] * matr[0][3 * (size - 1) + 2];
                    } else if (x + i < 0) {                                                                  //prima linie                         
                        sumR += (double)kernel[i + radius][j + radius] * matr[0][3 * (y + j)];
                        sumG += (double)kernel[i + radius][j + radius] * matr[0][3 * (y + j) + 1];
                        sumB += (double)kernel[i + radius][j + radius] * matr[0][3 * (y + j) + 2];
                    } else if (x + i >= size && y + j >= size) {                                             //colt dreapta jos
                        sumR += (double)kernel[i + radius][j + radius] * matr[size - 1][3 * (size - 1)];
                        sumG += (double)kernel[i + radius][j + radius] * matr[size - 1][3 * (size - 1) + 1];
                        sumB += (double)kernel[i + radius][j + radius] * matr[size - 1][3 * (size - 1) + 2];
                    } else if (y + j >= size) {                                                              //ultima coloana
                        sumR += (double)kernel[i + radius][j + radius] * matr[x + i][3 * (size - 1)];
                        sumG += (double)kernel[i + radius][j + radius] * matr[x + i][3 * (size - 1) + 1];
                        sumB += (double)kernel[i + radius][j + radius] * matr[x + i][3 * (size - 1) + 2];
                    } else if (x + i >= size && y + j < 0 ) {                                                //colt stanga jos
                        sumR += (double)kernel[i + radius][j + radius] * matr[size - 1][0];
                        sumG += (double)kernel[i + radius][j + radius] * matr[size - 1][1];
                        sumB += (double)kernel[i + radius][j + radius] * matr[size - 1][2];
                    } else if (x + i >= size) {                                                              //ultima linie
                        sumR += (double)kernel[i + radius][j + radius] * matr[size - 1][3 * (y + j)];
                        sumG += (double)kernel[i + radius][j + radius] * matr[size - 1][3 * (y + j) + 1];
                        sumB += (double)kernel[i + radius][j + radius] * matr[size - 1][3 * (y + j) + 2];
                    } else {                                                                                 //prima coloana (y + j < 0)
                        sumR += (double)kernel[i + radius][j + radius] * matr[x + i][0];
                        sumG += (double)kernel[i + radius][j + radius] * matr[x + i][1];
                        sumB += (double)kernel[i + radius][j + radius] * matr[x + i][2];
                    }
                }
            }
            matr[x][3 * y] = (unsigned char)(round(sumR));
            matr[x][3 * y + 1] = (unsigned char)(round(sumG));
            matr[x][3 * y + 2] = (unsigned char)(round(sumB));
        }
    }
    for (x = 0; x < kernelWidth; x++) {
        free(kernel[x]);
    }
    free(kernel);
    stop = clock();
	printf("time: %lf ms\n",(double)(((stop-start) * 1000 ) / CLOCKS_PER_SEC));
}

void Gaussian_blur_iter(unsigned char **matr, unsigned int size, unsigned int radius, unsigned int iter) {
    register int i;
    for(i = 0; i < iter; i++) {
        Gaussian_blur(matr, size, radius);
    }
}

void Box_blur(unsigned char **matr, unsigned int size) {
    // clock_t start, stop;
    // start = clock();
    double sumR, sumG, sumB, counter;
    register int x, y, i, j;
    for (x = 0; x < size; x++) {
        for (y = 0; y < size; y++) {
            sumR = 0;
            sumG = 0;
            sumB = 0;
            counter = 0;
            for (i = -1; i <= 1; i++) {
                for (j = -1; j <= 1; j++) {
                    if (x + i >= 0 && x + i < size && y + j >= 0 && y + j < size) {     //interior
                        sumR += (double)matr[x + i][3 * (y+ j)];
                        sumG += (double)matr[x + i][3 * (y + j) + 1];
                        sumB += (double)matr[x + i][3 * (y + j) + 2];
                        counter++;
                    } else {
                        continue;
                    }
                }
            }
            matr[x][3 * y] = (unsigned char)round(sumR / counter);
            matr[x][3 * y + 1] = (unsigned char)round(sumG / counter);
            matr[x][3 * y + 2] = (unsigned char)round(sumB / counter);
        }
    }
    // stop = clock();
	// printf("time: %lf ms\n",(double)(((stop-start) * 1000 ) / CLOCKS_PER_SEC));
}

void Box_blur_iter(unsigned char **matr, unsigned int size, unsigned int iter) {
    clock_t start, stop;
    start = clock();
    register int i;
    for(i = 0; i < iter; i++) {
        Box_blur(matr, size);
    }
    stop = clock();
	printf("time: %lf ms\n",(double)(((stop-start) * 1000 ) / CLOCKS_PER_SEC));
}