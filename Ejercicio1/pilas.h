#ifndef __PILAS_H__
#define __PILAS_H__

#include <stddef.h>
#include "glist.h"

struct _Pila {
GList inicio;
};
typedef struct _Pila *Pila;

/*
    Crea una pila vacia
*/
Pila pila_crear();

/*
    Destruye una pila, liberando el espacio ocupado por esta
*/
void pila_destruir(Pila pila, FuncionDestructora fdestroy);

/*
    Devuelve 1 o 0 dependiendo si la pila tiene elementos o no
*/
int pila_es_vacia(Pila pila);

/*
    Devuelve el ultimo elemento de la pila, pero no lo elimina
*/
void* pila_tope(Pila pila);

/*
    Agrega un elemento a la pila
*/
void pila_apilar(Pila pila, void* dato, FuncionCopia fcopy);

/*
    Popea el ultimo elemento de la pila
*/
void pila_desapilar(Pila pila, FuncionDestructora fdestroy);

#endif /* __PILAS_H__ */
