#include "heaps.h"
#include <assert.h>
#include <stdio.h>

static void cambiar(Celda elem1, Celda elem2) {
    void* dato1 = elem1->dato;
    void* llave1 = elem1->llave;
 
    elem1->dato = elem2->dato;
    elem1->llave = elem2->llave;

    elem2->dato = dato1;
    elem2->llave = llave1;
}

BHeap bheap_crear(size_t capacidad, FuncionComparadora comp_llave, FuncionComparadora comp_dato , FuncionDestructora destroy_llave, FuncionDestructora destroy_dato, FuncionCopia copy_llave, FuncionCopia copy_dato){
    BHeap heap = malloc(sizeof(struct _BHeap));
    assert(heap != NULL);
    heap->arr = malloc(sizeof(void *) * (capacidad + 1));
    assert(heap->arr != NULL);

    heap->capacidad = capacidad;
    heap->comp_dato = comp_dato;
    heap->destroy_dato = destroy_dato;
    heap->copy_dato = copy_dato;

    heap->destroy_llave = destroy_llave;
    heap->comp_llave = comp_llave;
    heap->copy_llave = copy_llave;

    heap->ultimo = 0;

    return heap;
}

void bheap_destruir(BHeap heap){
    for(int i = 1; i <= heap->ultimo; i++){
        heap->destroy_dato(heap->arr[i]->dato);
        heap->destroy_llave(heap->arr[i]->llave);
        free(heap->arr[i]);
    }
    free(heap->arr);
    free(heap);
}

int bheap_es_vacio(BHeap heap){
    return heap->ultimo == 0;
}

void bheap_recorrer(BHeap heap, FuncionVisitante visit){
    for(int i = 1; i <= heap->ultimo; i++){
        visit(heap->arr[i]->dato);
        visit(heap->arr[i]->llave);
        fprintf(stderr, " ");
    }
}

//Hace flotar un nodo 
static void _bheap_flotar(BHeap heap, int pos){
    if (pos > 1)
        for (; pos > 1 && heap->comp_llave(heap->arr[pos]->llave, heap->arr[pos / 2]->llave) < 0; pos /= 2){
            cambiar(heap->arr[pos], heap->arr[pos / 2]);
        }
}

//Hace hundir un nodo 
static void _bheap_hundir(BHeap heap, int pos){
    int max_pos = pos;
    int izq = pos * 2, der = pos * 2 + 1;

    if(izq <= heap->ultimo && heap->comp_llave(heap->arr[max_pos]->llave, heap->arr[izq]->llave) > 0)
        max_pos = izq;
    
    if(der <= heap->ultimo && heap->comp_llave(heap->arr[max_pos]->llave, heap->arr[der]->llave) > 0)
        max_pos = der;
    
    if(max_pos != pos){
        cambiar(heap->arr[pos], heap->arr[max_pos]);

        _bheap_hundir(heap, max_pos);
    }
}

void bheap_insertar(BHeap heap, void* dato, void* llave){
    if(heap->ultimo < heap->capacidad){
        Celda nuevaCelda = malloc(sizeof(struct _Celda));
        assert(nuevaCelda != NULL);

        nuevaCelda->dato = heap->copy_dato(dato);
        nuevaCelda->llave = heap->copy_llave(llave);
        heap->arr[++heap->ultimo] = nuevaCelda;
    }

    _bheap_flotar(heap, heap->ultimo);
}

void bheap_eliminar(BHeap heap, void* dato){
    int dato_pos = -1;
    for(int i = 1; i <= heap->ultimo && dato_pos == -1; i++){
        if(heap->comp_dato(heap->arr[i]->dato, dato) == 0){
            dato_pos = i;
        }
    }
    
    if(dato_pos != -1){
        if(dato_pos == heap->ultimo){
            heap->destroy_dato(heap->arr[heap->ultimo]->dato);
            heap->destroy_llave(heap->arr[heap->ultimo]->llave);
            free(heap->arr[heap->ultimo--]);
        }
        else{
            //Pongo el elem al final
            cambiar(heap->arr[dato_pos], heap->arr[heap->ultimo]);

            //Libero el elemento ahroa en el final
            heap->destroy_dato(heap->arr[heap->ultimo]->dato);
            heap->destroy_llave(heap->arr[heap->ultimo]->llave);
            free(heap->arr[heap->ultimo--]);

            if(dato_pos > 1 && heap->comp_llave(heap->arr[dato_pos]->llave, heap->arr[dato_pos / 2]->llave) < 0)
                _bheap_flotar(heap, dato_pos);
            else
                _bheap_hundir(heap, dato_pos);
        }
    }
}

void bheap_actualizar_llave(BHeap heap, void* dato, void* llave){
    int dato_pos = -1;
    //Busco el dato
    for(int i = 1; i <= heap->ultimo && dato_pos == -1; i++){
        if(heap->comp_dato(heap->arr[i]->dato, dato) == 0){
            //Cambio su llave
            heap->destroy_llave(heap->arr[i]->llave);
            heap->arr[i]->llave = heap->copy_llave(llave);

            //Terminamos el loop y sacamos el indice que actualizamos
            dato_pos = i;
        }
    }

    //Cuando actualizo llaves es porque las hago mas altas, asi que ahora queda hundir mi nueva llave.
    if(dato_pos != -1){
        _bheap_hundir(heap, dato_pos);
    }
}

int cola_prioridad_es_vacia(BHeap cola){
    return cola->ultimo == 0;
}

void *cola_prioridad_maxima_llave(BHeap cola){
    if(!cola_prioridad_es_vacia(cola))
        return cola->arr[1]->llave;
    return NULL;
}

void *cola_prioridad_maximo_dato(BHeap cola){
    if(!cola_prioridad_es_vacia(cola))
        return cola->arr[1]->dato;
    return NULL;
}

void cola_prioridad_eliminar_maximo(BHeap cola){
    bheap_eliminar(cola, cola->arr[1]->dato);
}

void cola_prioridad_insertar(BHeap cola, void* dato, void* llave){
    bheap_insertar(cola, dato, llave);
}

void cola_prioridad_actualizar_llave(BHeap cola, void* dato, void* llave){
    bheap_actualizar_llave(cola, dato, llave);
}

