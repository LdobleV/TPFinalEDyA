#include <stdio.h>
#include <stdlib.h>
#include "LibreriaRobot.h"
#include <time.h>
#include <assert.h>

void destruir_dir(char* dir){
    free(dir);
}

size_t redimensionar_camino(char** camino, size_t maxCaminoL){
    size_t nuevaLongitud = maxCaminoL * 2;
    *camino = realloc(*camino, sizeof(char) * nuevaLongitud);
    assert(*camino != NULL);
    return nuevaLongitud;
}

int main(int argc, char *argv[])
{
    //Primero chequeamos que sea valida la entrada al programa
    if (argc != 2)
    {
        fprintf(stderr, "Numero incorrecto de argumentos, Uso: ./robot\n");
        exit(-1);
    }
    FILE *archivoLaberinto;
    char *nombreLaberinto = argv[1];
    archivoLaberinto = fopen(nombreLaberinto, "r");
    
    //Chequeamos que el archivo se haya abierto de forma satisfactoria
    if (archivoLaberinto == NULL){
        fprintf(stderr, "%s: No existe el archivo dado / no posee permisos para leerlo\n", argv[1]);
        exit(-1);
    }


    //HACER QUE SE DENIEGUE EL ARCHIVO SI NO CUMPLE CON LA CONSIGNA
    int alto, ancho, rangoSensor;
    if(fscanf(archivoLaberinto ,"%d %d %d", &alto , &ancho, &rangoSensor) != 3){
        fprintf(stderr, "Error en el formato del archivo\n");
        exit(-1);
    }

    int inicialY, inicialX;
    if(fscanf(archivoLaberinto, "%d %d", &inicialY , &inicialX) != 2){
        fprintf(stderr, "Error en el formato del archivo\n");
        exit(-1);
    }

    Pos2D posInicial;
    posInicial.y = inicialY;
    posInicial.x = inicialX;

    int finalY, finalX;
    if(fscanf(archivoLaberinto, "%d %d", &finalY , &finalX) != 2){
        fprintf(stderr, "Error en el formato del archivo\n");
        exit(-1);
    }

    Pos2D posDestino;
    posDestino.y = finalY;
    posDestino.x = finalX;

    int maxCaminoLen = 100;
    //Terminamos de chequear el archivo, ahora pasamos a la creacion y resolucion del laberinto
    Laberinto laberinto = armar_laberinto(archivoLaberinto, alto, ancho);
    fclose(archivoLaberinto);

    if (laberinto == NULL)
        exit(-1);

    //Seedeamos el random
    srand(time(NULL));
    
    //Inicializamos el robot
    Robot robot = robot_inicializar(posInicial, posDestino);

    char* camino = malloc(sizeof(char) * maxCaminoLen);
    assert(camino != NULL);
    size_t caminoLongitud = 0;

    camino[caminoLongitud] = '\0';

    //Mientras el robot no termine, trata de efectuar movimientos
    while(!robot_llego_a_destino(robot)){
        if(caminoLongitud >= maxCaminoLen - 2){
                maxCaminoLen = redimensionar_camino(&camino, maxCaminoLen);
        }
        caminoLongitud = robot_elegir_movimiento(robot, laberinto, camino, caminoLongitud);       
    }

    printf("%s", camino);
    eliminar_tabla(robot->informacion);
    pila_destruir(robot->pila, (FuncionDestructora)destruir_dir);
    for(int i = 0; i < laberinto->alto; i++){
        free(laberinto->mapa[i]);
    }
    free(robot);
    free(laberinto->mapa);
    free(laberinto);
    free(camino);
}