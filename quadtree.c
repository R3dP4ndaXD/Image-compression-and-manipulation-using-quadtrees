/*IACOB Andrei - 313CB*/
#include"quadtree_comp.h"
#include"quadtree_decomp.h"
#include"image_processing.h"
#include<string.h>


int main(int argc, char* arcv[]) {
    if(argc == 1) {
        printf("Nu ai introdus parametrii\n");
        return 0;
    }
    else if(!strncmp(arcv[1],"-c", 2)) {
        unsigned int n = 0;
        unsigned char **matr = read_matr(arcv[3], &n);
        if (!matr) {
            return 0;
        }
        unsigned int score = (unsigned int)strtoul(arcv[2], NULL, 0);
        QTree T = compress(matr, score, 0, 0, n);
        if (!T) {
            free_matr(&matr, n);
            return 0;
        }
        if (arcv[1][2] == '1') {
            Task1(T, n, arcv[4]);
        } else if (arcv[1][2] == '2') {
            Task2(T, n, arcv[4]); 

        }
        free_matr(&matr, n);
        free_tree(T);
        T = NULL;
    } else if (!strncmp(arcv[1],"-d", 2)) {
        Task3(arcv[2], arcv[3]);
    } else if(!strncmp(arcv[1],"-p1", 3)) {
        // ./quadtree -p1 <factor de compresie> <fisier de intrare> <fisier comprimat> <fisier decomprimat> 
        unsigned int n = 0;
        unsigned char **matr = read_matr(arcv[3], &n);
        if (!matr) {
            return 0;
        }
        unsigned int score = (unsigned int)strtoul(arcv[2], NULL, 0);
        //Gaussian_blur(matr, n, 3);                    //se decomenteaza si se modifica parmetrii dupa caz
        Box_blur_iter(matr, n, 25);
        QTree T = compress(matr, score, 0, 0, n);
        if (!T) {
            free_matr(&matr, n);
            return 0;
        }
        //flip_orizontal(T);
        //flip_vertical(T);                             //se decomenteaza in functie operatiunea dorita  
        //flip_dreapta(T);
        //flip_stanga(T);
        //gray_scaling(T);
        //negativ(T);
        //sepia(T);
        Task2(T, n, arcv[4]);
        Task3(arcv[4], arcv[5]); 
        free_matr(&matr, n);
        free_tree(T);
        T = NULL;
    } else if(!strncmp(arcv[1],"-p2", 3)) {
        // ./quadtree -p1 <factor de compresie1> <factor de compresie2> <fisier de intrare1 <fisier de intrare>> <fisier comprimat> <fisier decomprimat>
        unsigned int n1, n2, n3;
        unsigned char **matr1 = read_matr(arcv[4], &n1);
        if (!matr1) {
            return 0;
        }
        unsigned char **matr2 = read_matr(arcv[5], &n2);
        if (!matr2) {
            free_matr(&matr1, n1);
            return 0;
        }
        unsigned int score1 = (unsigned int)strtoul(arcv[2], NULL, 0);
        unsigned int score2 = (unsigned int)strtoul(arcv[3], NULL, 0);
        QTree T1 = compress(matr1, score1, 0, 0, n1);
        if (!T1) {
            free_matr(&matr2, n2);
            free_matr(&matr1, n1);
            return 0;
        }
        QTree T2 = compress(matr2, score2, 0, 0, n2);
        if (!T2) {
            free_tree(T1);
            free_matr(&matr2, n2);
            free_matr(&matr1, n1);
            T1 = NULL;
            return 0;
        }
        if(n1 >= n2) {
            n3 = n1;
        } else {
            n3 = n2;
        }
        //QTree T3 = overlap(T1, T2);
        //QTree T3 = overlay(T1, T2);           //se decomenteza in functie operatiunea dorita
        QTree T3 = intersect(T1, T2);
        minimizare(T3);
        printf("marinea laturii blocului maxim: %i\nmarimea laturii imaginii:%i\n", max_block(T3, n3), n3);
        Task2(T3, n3, arcv[6]);
        Task3(arcv[6], arcv[7]); 
        free_matr(&matr1, n1);
        free_matr(&matr2, n2);
        free_tree(T1);
        free_tree(T2);
        free_tree(T3);
        T1 = NULL;
        T2 = NULL;
        T3 = NULL;
    } else if(!strncmp(arcv[1],"-l", 2)) {
        unsigned int n = 0;
        unsigned char **matr1 = read_matr(arcv[3], &n);
        if (!matr1) {
            return 0;
        }
        unsigned int score = (unsigned int)strtoul(arcv[2], NULL, 0);
        unsigned int nr_labels = 0;
        unsigned char **matr2 = ConnectedComponentLabelling(matr1, n, &nr_labels);
        printf("%i labels\n", nr_labels);
        QTree T = compress(matr2, score, 0, 0, n);
        if (!T) {
            free_matr(&matr1, n);
            free_matr(&matr2, n);
            return 0;
        }
        Task2(T, n, arcv[4]);
        Task3(arcv[4], arcv[5]); 
        free_tree(T);
        T = NULL;
        free_matr(&matr1, n);
        free_matr(&matr2, n);
        return 0;
    }
    return 0;
}