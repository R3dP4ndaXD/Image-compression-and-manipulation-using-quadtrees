/*IACOB Andrei - 313CB*/
#include"quadtree_decomp.h"

/*
    Construieste arborele pe baza informatiilor din fisier si foloseste o coada in care retine nodurile
    neterminale pentru a sti unde trebuie legate mai departe seturile de cate 4 fii care vor fi citite
*/
QTree read_tree(char *file, unsigned int *n) {
    FILE *fbin = fopen(file, "rb");
    if (!fbin) {
        printf("Nu ai introdus un fisier de intrare valid\n");
        return NULL;
    }
    fread(n, sizeof(unsigned int), 1, fbin);
    QTree T = calloc(1, sizeof(Node));
    if (!T) {
        fclose(fbin);
        return NULL;
    }
    unsigned char type;
    fread(&type, sizeof(unsigned char), 1, fbin);
    if (type == 1) {
        fread(T, sizeof(unsigned char), 3, fbin);
        fclose(fbin);
        return T;
    }
    TCoada *c = InitQ();
    if (!c) {
        free(T);
        T = NULL;
        fclose(fbin);
        return NULL;
    }
    if (!IntrQ(c, T)) {
        free(c);
        c = NULL;
        free(T);
        T = NULL;
        fclose(fbin);
        return NULL;
    }
    
    QTree p;
    while (c->inc) {
        ExtQ(c, &p, sizeof(QTree));
        p->q1 = calloc(1, sizeof(Node));
        if(!p->q1) {
            DistrugeQ(&c);
            free_tree(T);
            T = NULL;
            fclose(fbin);
            return NULL;
        }
        fread(&type, sizeof(unsigned char), 1, fbin);
        if(type == 0) {
            if(!IntrQ(c, p->q1)) {
                DistrugeQ(&c);
                free_tree(T);
                T = NULL;
                fclose(fbin);
                return NULL;                
            }
        } else {;
            fread(p->q1, sizeof(unsigned char), 3, fbin);
        }
        p->q2 = calloc(1, sizeof(Node));
        if(!p->q2) {
            DistrugeQ(&c);
            free_tree(T);
            T = NULL;
            fclose(fbin);
            return NULL;
        }
        fread(&type, sizeof(unsigned char), 1, fbin);
        if(type == 0) {
            if(!IntrQ(c, p->q2)) {
                DistrugeQ(&c);
                free_tree(T);
                T = NULL;
                fclose(fbin);
                return NULL;                
            }
        } else {
            fread(p->q2, sizeof(unsigned char), 3, fbin);
        }
        p->q3 = calloc(1, sizeof(Node));
        if(!p->q3) {
            DistrugeQ(&c);
            free_tree(T);
            T = NULL;
            fclose(fbin);
            return NULL;
        }
        fread(&type, sizeof(unsigned char), 1, fbin);
        if(type == 0) {
            if(!IntrQ(c, p->q3)) {
                DistrugeQ(&c);
                free_tree(T);
                T = NULL;
                fclose(fbin);
                return NULL;                
            }
        } else {
            fread(p->q3, sizeof(unsigned char), 3, fbin);
        }
        p->q4 = calloc(1, sizeof(Node));
        if(!p->q4) {
            DistrugeQ(&c);
            free_tree(T);
            T = NULL;
            fclose(fbin);
            return NULL;
        }
        fread(&type, sizeof(unsigned char), 1, fbin);
        if(type == 0) {
            if(!IntrQ(c, p->q4)) {
                DistrugeQ(&c);
                free_tree(T);
                T = NULL;
                fclose(fbin);
                return NULL;                
            }
        } else {
            fread(p->q4, sizeof(unsigned char), 3, fbin);
        }
    }
    free(c);
    c = NULL;
    fclose(fbin);
    return T;
}

/*
    Se parcurge recursiv arborele pana la frunze si se "coloreaza" matricea la iesirea din recursivitate.
    Coodonatele si marimea zonei de colorat vin ca parametii ai functie si sunt determinati usor
    prin raportare la nodul parinte si la nivelul pe care se afla frunza(nivelul recursivitatii)
*/
void decompress(QTree T, unsigned char **matr, unsigned int x, unsigned int y, unsigned int size) {
    unsigned int i, j;
    if(!T->q1) {
        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                matr[x + i][3 * (y + j)] = T->R;;
                matr[x + i][3 * (y + j) + 1] = T->G;;
                matr[x + i][3 * (y + j) + 2] = T->B;
            }
        
        }
    } else {
        decompress(T->q1, matr, x, y, size/2);
        decompress(T->q2, matr, x, y + size/2, size/2);
        decompress(T->q3, matr, x + size/2, y + size/2, size/2);
        decompress(T->q4, matr, x + size/2, y, size/2);
    }
}

void Task3(char* file1, char* file2) {
    unsigned int n, i;
    QTree T = read_tree(file1, &n);
    if(!T) {
        return;
    }
    unsigned char **matr = calloc(n, sizeof(unsigned char*));
    if (!matr) {
        free_tree(T);
        T = NULL;
        return;
    }
    for (i = 0; i < n; i++) {
        matr[i]= calloc(3 * n, sizeof(unsigned char));
        if (!matr[i]) {
            free_matr(&matr, i);
            free_tree(T);
            T = NULL;
            return;
        }
    }
    FILE *fbo = fopen(file2, "wb");
    fprintf(fbo, "P6\n%u %u\n255\n", n, n);
    decompress(T, matr, 0, 0, n);
    free_tree(T);
    T = NULL;
    for (i = 0; i < n; i++) {
        fwrite(matr[i], sizeof(unsigned char), 3 * n, fbo);
    }
    free_matr(&matr, n);
    fclose(fbo);
}
