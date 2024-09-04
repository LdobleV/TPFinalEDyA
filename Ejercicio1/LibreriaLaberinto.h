#ifndef __LIBRERIALABERINTO_H__
    #define __LIBRERIALABERINTO_H__
    #include <stdio.h>
    #include <stdlib.h>

    typedef struct _Pos2D{
        int y;
        int x;
    } Pos2D;

    typedef struct L{
        char** mapa;
        int alto;
        int ancho;
    } _Laberinto;

    typedef _Laberinto* Laberinto;
    
    /*
        Devuelve una Pos2D dados sus argumentos
    */
    Pos2D nueva_pos2D(int y, int x);

    /*
        Crea un laberinto sin obstaculos
    */
    Laberinto crear_laberinto_libre();

    /*
        Recibe el archivo del laberinto y devuelve la estructura formada del laberinto
    */
    Laberinto armar_laberinto(FILE* archivoLaberinto, int alto, int ancho);

    /*
        Retorna 1 si la posicion es valida en el laberinto dado, 0 en caso contrario
    */
    int posicion_es_valida(Laberinto laberinto, Pos2D posicion);

#endif //__LIBRERIALABERINTO_H__