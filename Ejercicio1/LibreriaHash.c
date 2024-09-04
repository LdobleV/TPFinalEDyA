#include "LibreriaHash.h"
#include <assert.h>

size_t funcion_hash(int y, int x, TablaHash tabla){
    return (x * 31 + y) % tabla->longitud;
}

void inicializar_tabla(TablaHash tabla, size_t longitud){
    tabla->tabla = malloc(sizeof(struct _Celda) * longitud);
    assert(tabla->tabla != NULL);
    tabla->longitud = longitud;
    tabla->cantUsada = 0;

    for(int i = 0; i < tabla->longitud; i++){
        tabla->tabla[i] = NULL;
    }
}

void eliminar_tabla(TablaHash tabla) {
    for (int i = 0; i < tabla->longitud; i++) {
        Celda celdaActual = tabla->tabla[i];
        while (celdaActual != NULL) {
            Celda temp = celdaActual;
            celdaActual = celdaActual->proxima;
            free(temp);
        }
    }
    free(tabla->tabla);
    free(tabla);
}

static void reescalar_tabla(TablaHash tabla, size_t nuevaLongitud) {
    Celda* viejaTabla = tabla->tabla;
    int viejaLongitud = tabla->longitud;
    tabla->tabla = malloc(sizeof(Celda) * nuevaLongitud);
    assert(tabla->tabla != NULL);

    for(int i = 0; i < nuevaLongitud; i++)
        tabla->tabla[i] = NULL;

    tabla->longitud = nuevaLongitud;
    // Iteramos sobre la tabla anterior para rearmarla con nueva longitud
    for (int i = 0; i < viejaLongitud; i++) {
        Celda celdaActual = viejaTabla[i];
        while (celdaActual != NULL) {
            // Calculamos un nuevo indice para la celda en base a la nueva longitud
            asignar_celda(tabla, celdaActual->pos);
            //No quiero que insertar al reescalar cuente para cantUsada de mi tabla
            tabla->cantUsada--;

            // Vamos al proximo elemento de mi tabla vieja y borramos lo q habia
            Celda celdaABorrar = celdaActual;
            celdaActual = celdaActual->proxima;
            free(celdaABorrar);
        }
    }
    // Liberamos la vieja tabla
    free(viejaTabla);
    // Actualizo la estructura de mi tabla
}

void asignar_celda(TablaHash tabla, Pos2D pos){
    size_t indice = funcion_hash(pos.y, pos.x, tabla);
    Celda celdaActual = tabla->tabla[indice];
    
    // Busco mi elemento en la lista enlazada
    while (celdaActual != NULL) {
        if (celdaActual->pos.x == pos.x && celdaActual->pos.y == pos.y) {
            celdaActual = NULL;
        }else
            celdaActual = celdaActual->proxima;
    }

    // SI no la encuentro, la creo y la meto en la lista
    Celda nuevaCelda = malloc(sizeof(struct _Celda));
    assert(nuevaCelda != NULL);
    nuevaCelda->pos.x = pos.x;
    nuevaCelda->pos.y = pos.y;
    nuevaCelda->proxima = tabla->tabla[indice];
    tabla->tabla[indice] = nuevaCelda;

    tabla->cantUsada++;
    if((float)tabla->cantUsada/(float)tabla->longitud > 3){
        reescalar_tabla(tabla, tabla->longitud * 4);
    }
}

int obtener_celda(TablaHash tabla, Pos2D pos){
    size_t indice = funcion_hash(pos.y, pos.x, tabla);
    Celda celdaActual = tabla->tabla[indice];

    int bandera = 0;
    // Busco en la lista enlazada si esta la posicion
    while (celdaActual != NULL) {
        if (celdaActual->pos.x == pos.x && celdaActual->pos.y == pos.y) {
            bandera = 1;
            celdaActual = NULL;
        }else
            celdaActual = celdaActual->proxima;
    }

    return bandera;
}