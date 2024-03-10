/*IACOB Andrei - 313CB*/
#include"queue.h"

TCoada* InitQ() {
    TCoada *c= malloc(sizeof(TCoada));
    if(!c) {
        return NULL;
    }
    c->inc = NULL;
    c->sf= NULL;
    return c;
}

int IntrQ(TCoada *c, void *elem) {
    TLista aux = malloc(sizeof(TCelula));
    if(!aux)
        return 0;
    aux->info = elem;
    //aux->info = malloc(size);
    //memcpy(aux->info, elem, size);
    aux->urm = NULL;
    if(c->sf){
        c->sf->urm=aux;
    } else {
        c->inc=aux;
    }
    c->sf=aux;
    return 1;
}
int ExtQ(TCoada *c, void *elem, size_t size) {
    if(!c->inc)
        return 0;
    TLista aux = c->inc;
    memcpy(elem ,&aux->info, size);
    c->inc = aux->urm;
    if(!c->inc) {
        c->sf = NULL;
    }
    free(aux);
    
    return 1;
}

void DistrugeQ(TCoada **c) {
    TLista p = (*c)->inc, aux;
    while (p) {
        aux = p;
        p = p->urm;
        free(aux);
    }
    free(*c);
    *c = NULL;
}
