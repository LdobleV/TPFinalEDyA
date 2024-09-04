#include "pilas.h"
#include <assert.h>
#include <stdlib.h>

/*
    Crea una pila vacia
*/
Pila pila_crear(){
    Pila result = malloc(sizeof(Pila));
    result->inicio = NULL;
    
    return result;
}


void pila_destruir(Pila pila, FuncionDestructora fdestroy){
    glist_destruir(pila->inicio, fdestroy);
    free(pila);
}


int pila_es_vacia(Pila pila){
    return pila->inicio == NULL;
}


void* pila_tope(Pila pila){
    GList nodo = pila->inicio;

    return nodo->data;
}


void pila_apilar(Pila pila, void* dato, FuncionCopia fcopy){
    pila->inicio = glist_agregar_inicio(pila->inicio, dato, fcopy);
}


void pila_desapilar(Pila pila, FuncionDestructora fdestroy){
    if(pila->inicio != NULL){
        GList aux = pila->inicio;
        pila->inicio = pila->inicio->next;
        fdestroy(aux->data);
        free(aux);
    }
}
