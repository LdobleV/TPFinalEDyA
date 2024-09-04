#include "LibreriaRobot.h"
#include <assert.h>
#include <stdio.h>

//FuncionDestructora para mi pila
static void destruir_dir(char* dir){
    free(dir);
}

//FuncionCopia para mi pila
static char* copiar_dir(char* dir){
    char* result = malloc(sizeof(char));
    assert(result != NULL);
    *result = *dir;
    return result;
}

static const int random_rango(const int min, const int max) {
    return rand() % (max - min + 1) + min;
}

char direccion_contraria(char direccion){
    char result = 'X';
    switch(direccion){
        case 'D':
            result = 'U';
            break;
        case 'U':
            result = 'D';
            break;
        case 'R':
            result = 'L';
            break;
        case 'L':
            result = 'R';
            break;
        default:
            break;
    }


    return result;
}


Robot robot_inicializar(Pos2D posicion, Pos2D destino){
    Robot robot = malloc(sizeof(_Robot));
    assert(robot != NULL);
    robot->posicion = posicion;
    robot->destino = destino;
    robot->pila = pila_crear();
    size_t laberintoAlto = posicion.y > destino.y ? posicion.y : destino.y;
    size_t laberintoAncho = posicion.y > destino.y ? posicion.y : destino.y;

    robot->informacion = malloc(sizeof(struct _TablaHash));
    assert(robot->informacion != NULL);
    inicializar_tabla(robot->informacion, laberintoAlto * laberintoAncho < 100 ? 100 : laberintoAlto * laberintoAncho);

    return robot;
}

int robot_llego_a_destino(Robot robot){
    return robot->destino.x == robot->posicion.x && robot->destino.y == robot->posicion.y;
}

int robot_mover_a_posicion(Robot robot, char direccion, Laberinto laberinto){
    Pos2D posicionAMoverse = robot->posicion;

    switch(direccion){
        case 'U':
            posicionAMoverse.y -= 1;
            break;
        case 'D':
            posicionAMoverse.y += 1;
            break;
        case 'L':
            posicionAMoverse.x -= 1;
            break;
        case 'R':
            posicionAMoverse.x += 1;
            break;
        default:
            return -1;
    }

    if(posicion_es_valida(laberinto, posicionAMoverse)){
        robot->posicion = posicionAMoverse;
        return 1;
    }
    return 0;
}

int robot_devolver_sig_casilla(Robot robot, char* direccion){
    char posibilidadesPrioritarias[2], posibilidades[3];
    size_t contPrioridad = 0, contPosibles = 0;
    //Identificadores para saber donde esta la salida para mi robot
    //diffX > 0 => A la izquierda, derecha caso contrario
    //diffY > 0 => Arriba, abajo caso contrario
    int diffX = robot->posicion.x - robot->destino.x;
    int diffY = robot->posicion.y - robot->destino.y;

    //Ahora chequeo si en cada una de mis 4 direcciones, la posicion esta en la tabla hash, si esta la puedo meter en mis posibilidades
    //En caso contrario las ignoro pues ya fui o hay un obstaculo
    
    if(!obtener_celda(robot->informacion, nueva_pos2D(robot->posicion.y + 1, robot->posicion.x))){
        //Chequeamos si es prioridad o no
        if(diffY < 0)
            posibilidadesPrioritarias[contPrioridad++] = 'D';
        else
            posibilidades[contPosibles++] = 'D';
    }

    if(!obtener_celda(robot->informacion, nueva_pos2D(robot->posicion.y, robot->posicion.x + 1))){
        //Chequeamos si es prioridad o no
        if(diffX < 0)
            posibilidadesPrioritarias[contPrioridad++] = 'R';
        else
            posibilidades[contPosibles++] = 'R';
    }

    if(!obtener_celda(robot->informacion, nueva_pos2D(robot->posicion.y - 1, robot->posicion.x))){
        //Chequeamos si es prioridad o no
        if(diffY > 0)
            posibilidadesPrioritarias[contPrioridad++] = 'U';
        else
            posibilidades[contPosibles++] = 'U';
    }

    if(!obtener_celda(robot->informacion, nueva_pos2D(robot->posicion.y, robot->posicion.x - 1))){
        //Chequeamos si es prioridad o no
        if(diffX > 0)
            posibilidadesPrioritarias[contPrioridad++] = 'L';
        else
            posibilidades[contPosibles++] = 'L';
    }

    //No hay casillas a las que nos queramos mover
    if(!contPrioridad && !contPosibles)
        return 0;

    if(contPrioridad)
        *direccion = posibilidadesPrioritarias[random_rango(0, contPrioridad - 1)];
    else
        *direccion = posibilidades[random_rango(0, contPosibles - 1)];
    
    return 1;
}

static Pos2D obtener_posicion_al_moverse(Pos2D pos, char dir){
    switch(dir){
        case 'U':
            return nueva_pos2D(pos.y - 1, pos.x);
        case 'D':
            return nueva_pos2D(pos.y + 1, pos.x);
        case 'L':
            return nueva_pos2D(pos.y, pos.x - 1);
        case 'R':
            return nueva_pos2D(pos.y, pos.x + 1);
        default:
            return pos;
    }
}

size_t robot_elegir_movimiento(Robot robot, Laberinto laberinto, char* camino, size_t caminoLongitud){
    char direccion = 'X';
    if(!robot_devolver_sig_casilla(robot , &direccion)){
        //Nos vamos a mover en la direccion contraria al ultimo movimiento hecho
        direccion = *(char*)pila_tope(robot->pila);
        
        pila_desapilar(robot->pila, (FuncionDestructora)destruir_dir);
            
        camino[caminoLongitud++] = direccion;
        camino[caminoLongitud] = '\0';

        robot_mover_a_posicion(robot, direccion, laberinto);

        return caminoLongitud;
    }
    else{
        asignar_celda(robot->informacion, obtener_posicion_al_moverse(robot->posicion, direccion));

        if (robot_mover_a_posicion(robot, direccion, laberinto)){
            //No me choque, apilo mi posicion anterior en la pila
            char direccionContraria = direccion_contraria(direccion);
            pila_apilar(robot->pila, &direccionContraria,  (FuncionCopia)copiar_dir);
            
            camino[caminoLongitud++] = direccion;
            camino[caminoLongitud] = '\0'; 
        }

        return caminoLongitud;
    }
}