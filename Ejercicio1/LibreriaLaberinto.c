#include "LibreriaLaberinto.h"
#include <assert.h>


Pos2D nueva_pos2D(int y, int x){
    Pos2D resultado;
    resultado.y = y;
    resultado.x = x;
    
    return resultado;
}


Laberinto armar_laberinto(FILE* archivo_laberinto, int alto, int ancho){
    Laberinto laberinto = malloc(sizeof(_Laberinto));
    laberinto->alto = alto;
    laberinto->ancho = ancho;
    laberinto->mapa = malloc(sizeof(char*) * alto);
    
    for(int y = 0; y < alto; y++){
        if(fgetc(archivo_laberinto) != '\n'){
            fprintf(stderr, "Error en el formato del laberinto\n");
            return NULL;
        }
        laberinto->mapa[y] = malloc(sizeof(char) * ancho);

        for(int x = 0; x < ancho; x++){
            laberinto->mapa[y][x] = fgetc(archivo_laberinto);

            if(laberinto->mapa[y][x] != '#' && laberinto->mapa[y][x] != '.'){
                fprintf(stderr, "Error en el formato del laberinto\n");
                return NULL;
            }
        }

    }

    return laberinto;
}

int posicion_es_valida(Laberinto laberinto, Pos2D posicion){
    if(posicion.x < laberinto->ancho && posicion.y < laberinto->alto && posicion.x >= 0 && posicion.y >= 0)
        if(laberinto->mapa[posicion.y][posicion.x] == '.')
            return 1;
    return 0;
}