#ifndef __HEAPS_H__
#define __HEAPS_H__
#include <stdlib.h>
#include <stddef.h>


typedef void (*FuncionDestructora)(void *dato);
typedef void *(*FuncionCopia)(void *dato);
typedef void (*FuncionVisitante)(void *dato);
typedef int (*FuncionComparadora)(void *datoA, void *datoB);

typedef struct _Celda {
    void *dato;
    //Se ordena segun el valor de la llave
    void *llave;
} *Celda;

typedef struct _BHeap {
    Celda *arr;
    int capacidad;
    int ultimo;
    /*
        La funcion comparadora (para datos) devuelve 0 si son iguales
        1 en caso contrario
    */
    FuncionComparadora comp_dato;
    FuncionDestructora destroy_dato;
    FuncionCopia copy_dato;

    /*
        La funcion comparadora (para llaves) devuelve -1 si llave1 < llave2, 1 si es mayor y 0 si son iguales
        Como estamos en un min_heap en la posicion 1 deberia estar el dato con la menor llave
    */
    FuncionComparadora comp_llave;
    FuncionDestructora destroy_llave;
    FuncionCopia copy_llave;
} *BHeap;

/*
    Recibe las funciones correspondientes y un tamaño para crear el heap
*/
BHeap bheap_crear(size_t capacidad, FuncionComparadora comp_llave, FuncionComparadora comp_dato , FuncionDestructora destroy_llave, FuncionDestructora destroy_dato, FuncionCopia copy_llave, FuncionCopia copy_dato);


/*
    Destruye un heap
*/
void bheap_destruir(BHeap heap);


/*
    Devuelve 1 si el heap recibido es vacio
*/
int bheap_es_vacio(BHeap heap);

/*
    Inserta un valor en el heap con la llave correspondiente, manteniendo la estructura
*/
void bheap_insertar(BHeap heap, void* dato, void* llave);

/*  
    Elimina un elemento del heap, buscandolo por el dato, manteniendo la estructura
*/
void bheap_eliminar(BHeap heap, void* dato);

/*
    Actualiza la llave de un elemento, buscandolo por dato, manteniendo la estructura
*/
void bheap_actualizar_llave(BHeap heap, void* dato, void* llave);

int cola_prioridad_es_vacia(BHeap cola);

void* cola_prioridad_maxima_llave(BHeap cola);

void* cola_prioridad_maximo_dato(BHeap cola);

void cola_prioridad_eliminar_maximo(BHeap cola);

void cola_prioridad_insertar(BHeap cola, void* dato, void* llave);

void cola_prioridad_actualizar_llave(BHeap cola, void* dato, void* llave);

#endif /* __HEAPS_H__ **/