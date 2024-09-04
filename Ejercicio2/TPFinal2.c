#include <stdio.h>
#include <stdlib.h>
#include "LibreriaRobot.h"
#include <time.h>
#include <assert.h>

size_t redimensionar_camino(char** camino, size_t maxCaminoL){
    size_t nuevaLongitud = maxCaminoL * 2;
    *camino = realloc(*camino, sizeof(char) * nuevaLongitud);
    assert(*camino != NULL);
    return nuevaLongitud;
}

int main()
{
    //Recepcion de los datos por parte del programa sensor
    int inicialX, inicialY, finalX, finalY, alto, ancho, maxSensorD;

    scanf ("%d%d%d", &alto , &ancho , &maxSensorD);

    scanf ("%d%d", &inicialY , &inicialX);

    scanf ("%d%d", &finalY, &finalX);

    Pos2D posInicial;
    posInicial.y = inicialY;
    posInicial.x = inicialX;

    Pos2D posDestino;
    posDestino.y = finalY;
    posDestino.x = finalX;
    
    Robot robot = robot_inicializar(posInicial, posDestino, alto, ancho);

    //Seedeamos el random
    srand(time(NULL));

    size_t caminoLongitud = 0, maxCaminoLen = robot->laberintoAlto * robot->laberintoAncho > 500 ? 500 : (robot->laberintoAlto * robot->laberintoAncho);
    char* camino = malloc(sizeof(char) * maxCaminoLen);
    assert(camino != NULL);

    //Obtengo el camino inicial para el robot, para un caso en el que se asume que no hay obstaculos en el laberinto
    robot_obtener_camino(robot);
    
    int direcciones[4] = {0, 0, 0, 0};

    Pos2D comienzo = robot->posicion;
    //Corre hasta que el robot llegue a su destino
    while (!robot_llego_a_destino(robot)){
        char direccion;

        //Obtenemos el proximo movimiento deseado del robot
        Pos2D posicionAMoverse = robot_obtener_movimiento(robot, &direccion);
        //Si desconocemos la casilla, llamamos al sensor
        if(robot->informacion[posicionAMoverse.y][posicionAMoverse.x].estado != DESCUBIERTA){
            printf("? %d %d\n", robot->posicion.y, robot->posicion.x);
            fflush(stdout);

            scanf("%d%d%d%d", &direcciones[0], &direcciones[1], &direcciones[2], &direcciones[3]);

            assert(direcciones[0] != 0 || direcciones[1] != 0 || direcciones[2] != 0 || direcciones[3] != 0);

            //En caso de aparecer un rango mayor al que piensa el robot, lo actualizamos antes actualizar cosas
            robot_actualizar_rango_sensor(robot, direcciones);

            int cambios = 0;
            //Chequeamos si hay cambios, actualizamos el km si hace falta
            for(int i = 0; i < 4; i++)
                if(direcciones[i] <= robot->rangoSensor){
                    robot->km += calcular_heuristica(robot->posicion, comienzo);
                    comienzo = robot->posicion;
                    cambios = 1;
                }

            //Aprovechamos la informacion del sensor, guardando los obstaculos y actualizando la informacion del mapa
            robot_actualizar_casilla_estado(robot, direcciones[0], 'U');
            robot_actualizar_casilla_estado(robot, direcciones[1], 'D');
            robot_actualizar_casilla_estado(robot, direcciones[2], 'L');
            robot_actualizar_casilla_estado(robot, direcciones[3], 'R');

            //Una vez actualizada la informacion, hay que recalcular el camino, en caso de que se haya cortado
            robot_obtener_camino(robot);
        }
        else{
            //Si se me pasa la memoria del camino, redimensiono
            if(caminoLongitud >= maxCaminoLen - 1){
                maxCaminoLen = redimensionar_camino(&camino, maxCaminoLen);
            }

            //Guardo la direccion en mi camino y me muevo a la posicion deseada
            camino[caminoLongitud++] = direccion;
            //fprintf(stderr, "\nMe muevo a: [%d, %d]", robot->posicion.y, robot->posicion.x);
            robot_mover_a_posicion(robot, direccion);
            camino[caminoLongitud] = '\0';
        }
    }
    //Termino y le paso el resultado al programa sensor
    printf("! %s\n", camino);
    fflush(stdout);
    
    for(int i = 0; i < robot->laberintoAncho; i++){
        free(robot->informacion[i]);
    }
    free(robot->informacion);
    bheap_destruir(robot->cola);
    free(robot);

    free(camino);
}