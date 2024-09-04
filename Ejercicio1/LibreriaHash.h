#ifndef __LIBRERIAHASH_H__
    #define __LIBRERIAHASH_H__
    #include "LibreriaLaberinto.h"

    typedef struct _Celda {
        Pos2D pos;
        struct _Celda *proxima;
    } *Celda;

    typedef struct _TablaHash {
        Celda *tabla;
        size_t longitud;
        size_t cantUsada;
    } *TablaHash;

    /*
        Determina el indice
    */
    size_t funcion_hash(int y, int x, TablaHash tabla);

    /*
        Incializa la tabla hash
    */
    void inicializar_tabla(TablaHash tabla, size_t longitud);

    /*
        Le asigna su celda a un elemento que quiero ingresar
    */
    void asignar_celda(TablaHash tabla, Pos2D pos);

    /*
        Obtiene si la celda esta o no en la tabla
    */
    int obtener_celda(TablaHash tabla, Pos2D pos);

    /*
        Elimina la tabla hash, liberando memoria
    */
    void eliminar_tabla(TablaHash tabla);

#endif //__LIBRERIALABERINTO_H__