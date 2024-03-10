/*IACOB Andrei - 313CB*/
#include"quadtree_comp.h"

/*
    Citeste antetul fisierului, aloca memorie pentru matrice si citeste matricea data in format binar
*/
unsigned char** read_matr(char *file, unsigned int *n) {
    FILE *fbin = fopen(file, "rb");
    if (!fbin) {
        printf("Nu ai introdus un fisier de intrare valid\n");
        return NULL;
    }
    char type[3];
    unsigned int max_val, i, m;
    fgets(type, 3, fbin);
    fscanf(fbin, "%i %i", n, &m);
    fscanf(fbin, "%i", &max_val);
    fseek(fbin, 1, SEEK_CUR);
    unsigned char **matr = calloc(m, sizeof(unsigned char *));
    if (!matr) {
        return NULL;
    }
    for (i = 0; i < m; i++) {
        matr[i] = calloc(3 * m, sizeof(unsigned char));
        if (!matr[i]) {
            free_matr(&matr, i);
            return NULL;
        }
        fread(matr[i], sizeof(unsigned char), 3 * m, fbin);
    }
    fclose(fbin);
    return matr;

}
/*
    Elibereaza memoria alocata matricei(intai vectorii liniilor si dupa vectorul pentru coloane)
*/
void free_matr(unsigned char*** matr, unsigned int n) {
    int i = 0;
    for (i = 0; i < n; i++) {
        free((*matr)[i]);
    }
    free(*matr);
    *matr = NULL;

}

/*
    pentru zona din matrice care incepe la coordonatele (x, y) si dimensiune size calculeaza culoarea
    medie(pe cele 3 canale) si dupa un scor al similaritatii culorilor din zona studiata
*/
void means(unsigned char **matr, unsigned int x, unsigned int y, unsigned int size, \
unsigned char *R, unsigned char *G, unsigned char *B, unsigned int *score) {
    unsigned long long R_mean = 0, G_mean = 0, B_mean = 0, mean = 0;
    unsigned int i = 0, j = 0;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            R_mean += matr[x + i][3 * (y + j)];
            G_mean += matr[x + i][3 * (y + j) + 1];
            B_mean += matr[x + i][3 * (y + j) + 2];
        }
        
    }
    R_mean /= size * size;
    G_mean /= size * size;
    B_mean /= size * size;
    *R = (unsigned char)R_mean;
    *G = (unsigned char)G_mean;
    *B = (unsigned char)B_mean;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            mean += (R_mean - matr[x + i][3 * (y + j)]) * (R_mean - matr[x + i][3 * (y + j)]) + (G_mean - matr[x + i][3 * (y + j) + 1])
            * (G_mean - matr[x + i][3 * (y + j) + 1]) + (B_mean - matr[x + i][3 * (y + j) + 2]) * (B_mean - matr[x + i][3 * (y + j) + 2]);
        }
        
    }
    mean /= 3 * size * size;
    *score = (unsigned int)mean;
}
/*
    Constructie recursiva a arborelui astfel:
    -calculeaza media
    -aloca nod
    -pentru score < factor se initializeaza nodul si se iese din recurivitate
    -pentru score > factor se apeleaza functia pe fiecare sfert din matricea primita, iar nodurile alocate mai departe
     se leaga de nodul din functia apelanta
    -funtia initiala va intoarce radacina arborelui construit
    In cazul unei erori de alocare de-a lungul apelurilor recursive, NULL-ul returnat va produce un lant de dealocari
    recursive pentru toti subarbori alocati pana in acel punct, iar functia intiala va intorace si ea NULL
*/
QTree compress(unsigned char **matr, unsigned int factor, unsigned int x, unsigned int y, unsigned int size) {
    unsigned char R = 0, G = 0, B = 0;  
    unsigned int score = 0;
    means(matr, x, y, size, &R, &G, &B, &score);
    QTree T = calloc(1, sizeof(Node));
    if (!T) {
        return NULL;
    }
    if (score <= factor) {
        T->R = R;
        T->G = G;
        T->B = B;
    }
    else {
        T->q1 = compress(matr, factor, x, y, size/2);
        if (!T->q1) {
            free(T);
            return NULL;
        }
        T->q2 = compress(matr, factor, x, y + size/2, size/2);
        if (!T->q2) {
            free_tree(T->q1);
            free(T);
            return NULL;
        }
        T->q3 = compress(matr, factor, x + size/2, y + size/2, size/2);
        if (!T->q3) {
            free_tree(T->q1);
            free_tree(T->q2);
            free(T);
            return NULL;
        }
        T->q4 = compress(matr, factor, x + size/2, y, size/2);
        if (!T->q4) {
            free_tree(T->q1);
            free_tree(T->q2);
            free_tree(T->q3);
            free(T);
            return NULL;
        }
    }
    return T;
}


/*
    Apeluri recursive pana se ajunge la frunze si dupa elibereaza menoria de jos in sus
    catre radacina la iesirea din recurisivitate
*/
void free_tree(QTree T) {
    if(!T) {
        return;
    }
    free_tree(T->q1);
    free_tree(T->q2);
    free_tree(T->q3);
    free_tree(T->q4);
    free(T);
}

/*
    Apeluri recursive pana la frunze si dupa la fiecara apel calculeaza nivelul maxim
    inapoi folosind valoarea maxima returnata de cele 4 apeluri incheiate anterior
*/
int nr_levels(QTree T) {
    if(!T) {
        return 0;
    }
    int n1, n2, n3, n4, max;
    n1 = nr_levels(T->q1);
    n2 = nr_levels(T->q2);
    n3 = nr_levels(T->q3);
    n4 = nr_levels(T->q4);
    if(n1 >= n2 && n1 >= n3 && n1 >= n4) {
        max = n1;
    } else if(n2 >= n1 && n2 >= n3 && n2 >= n4) {
        max = n2;
    } else if(n3 >= n1 && n3 >= n2 && n3 >= n4) {
        max = n3;
    } else {
        max = n4;
    }
    return 1 + max;
}
/*
    Apeluri recursive pana la frunze, si insumarea tuturor rezultatelor la iesirea din recusivitate
*/
int nr_leaves(QTree T) {
    if(!T) 
        return 0;
    if(!T->q1 && !T->q2 && !T->q3 && !T->q4)
        return 1;
    return nr_leaves(T->q1) + nr_leaves(T->q2) + nr_leaves(T->q3) + nr_leaves(T->q4);
}

/*
    Apeluri recursive pana la frunze si dupa la fiecara apel calculeaza nivelul minim mai departe
    folosind valoarea minima returnata de cele 4 apeluri incheiate anterior
*/
int first_leaf_level(QTree T) {
    if(!T) 
        return -1;
    if(!T->q1)
        return 0;
    int n1, n2, n3, n4, min;
    n1 = first_leaf_level(T->q1);
    n2 = first_leaf_level(T->q2);
    n3 = first_leaf_level(T->q3);
    n4 = first_leaf_level(T->q4);
    if(n1 <= n2 && n1 <= n3 && n1 <= n4) {
        min = n1;
    } else if(n2 <= n1 && n2 <= n3 && n2 <= n4) {
        min = n2;
    } else if(n3 <= n1 && n3 <= n2 && n3 <= n4) {
        min = n3;
    } else {
        min = n4;
    }
    return 1 + min;
        
}
/*
    Marimea laturii blocului maxim
*/
int max_block(QTree T, unsigned int n) {
    int level = first_leaf_level(T);
    return n >> level;
}

void Task1(QTree T, unsigned int n, char* file) {
    FILE *fo = fopen(file, "w");
    if (!fo) {
        printf("Nu ai introdus un fisier de iesire valid\n");
        return;
    }
    fprintf(fo, "%i\n", nr_levels(T));
    fprintf(fo, "%i\n", nr_leaves(T));
    fprintf(fo, "%i\n", max_block(T, n));

    fclose(fo);
}

/*
    Se parcuge arborele pe niveluri folosind o coada cu pointeri la celulele arborelui astfel:
    -se pune radacina in coada
    -se extrage cate un nod din coada
    -pentru fiecare nod analizat care nu e frunza, i se pune fii in coada
    -se scrie in fisier informatiile despre nodului parinte
*/
void Task2(QTree T, unsigned int n, char* file) {
    FILE *fbo = fopen(file, "wb");
    if (!fbo) {
        printf("Nu ai introdus un fisier de iesire valid\n");
        return;
    }
    fwrite(&n, sizeof(unsigned int), 1, fbo);
    TCoada *c = InitQ();
    if (!c) {
        fclose(fbo);
        return;
    }
    
    QTree p = NULL;
    unsigned char type = 0;
    if (!IntrQ(c, T)) {
        free(c);
        c = NULL;
        fclose(fbo);
        return;
    }
    //printf("aici\n");
    while (c->inc) {
        ExtQ(c, &p, sizeof(QTree));
        //printf("aici\n");
        if (p->q1) {
            
            type = 0;
            if (!IntrQ(c, p->q1)) {
                DistrugeQ(&c);
                fclose(fbo);
                return;
            }
            if (!IntrQ(c, p->q2)) {
                DistrugeQ(&c);
                fclose(fbo);
                return;
            }
            if (!IntrQ(c, p->q3)) {
                DistrugeQ(&c);
                fclose(fbo);
                return;
            }
            if (!IntrQ(c, p->q4)) {
                DistrugeQ(&c);
                fclose(fbo);
                return;
            }
        } else {
            type = 1;
        }
        fwrite(&type, sizeof(unsigned char), 1, fbo);
        if (type == 1) {
            fwrite(p, sizeof(unsigned char), 3, fbo);
        }
    }
    free(c);
    c = NULL;
    fclose(fbo);
}