#include "LibreriaRobot.h"
#include <assert.h>
#include <stdio.h>


/*
    Devuelve que es infinito para mi caso, lo defini como el (alto * ancho + 1) * 2 del laberinto pues es una distancia inalcanzable para cualquier laberinto
*/
static int inf(Robot robot){
    return (robot->laberintoAlto * robot->laberintoAncho + 1) * 2;
}

/*
    Constructor para Pos2D
*/
static Pos2D nueva_pos2D(int y, int x){
    Pos2D resultado;
    resultado.y = y;
    resultado.x = x;
    
    return resultado;
}

/*
    Funcion copia para mi heap
*/
static Pos2D *copiar_pos2D(Pos2D *pos){
    if (pos == NULL) return NULL;
    
    Pos2D *result = malloc(sizeof(Pos2D));
    assert(result != NULL);
    result->x = pos->x;
    result->y = pos->y;

    return result;
}

/*
    Funcion destructora para mi heap
*/
static void destruir_pos2D(Pos2D *pos){
    if (pos != NULL)
        free(pos);
}

//Devuelve 0 si son iguales 1 en caso contrario (Funcion comparadora de datos en mi heap)
static int comparar_posiciones(Pos2D *pos1, Pos2D *pos2) {
    if (pos1->x == pos2->x && pos1->y == pos2->y)
        return 0;  // Son iguales
    return 1;  // Son diferentes
}

//Llaves menores son mas prioritarias!
//Devuelve 1 si llave1 < llave2, -1 en caso contrario y 0 si son iguales
//(Funcion comparadora de llaves en mi heap)
static int comparar_llaves(Pos2D *llave1, Pos2D *llave2){
    if (llave1 == NULL)
        return 0;

    if (llave1->y < llave2->y)
        return -1;
    else if (llave1->y > llave2->y)
        return 1;
    
    if (llave1->x < llave2->x)
        return -1;
    else if(llave1->x > llave2->x)
        return 1;

    return 0;
}

static int random_rango(const int min, const int max) {
    return rand() % (max - min + 1) + min;
}

int calcular_heuristica(Pos2D pos1, Pos2D pos2){
    return abs(pos1.y - pos2.y) + abs(pos1.x - pos2.x);
}   

/*
    Calcula la llave de una casilla, la cual consta de una tupla que sigue la formula dada por el codigo
*/
static Pos2D calcular_llave(Robot robot, Pos2D pos){
    int rhs = robot->informacion[pos.y][pos.x].rhs;
    int g = robot->informacion[pos.y][pos.x].g;
    return nueva_pos2D((g < rhs ? g : rhs) + calcular_heuristica(robot->posicion, pos) + robot->km, (g < rhs ? g : rhs));
}

/*
    Devuelve el costo de moverse de pos1 a pos2, y viceversa pues en nuestro caso este costo es bidireccional
    Si se puede mover el costo es 1, si no se puede el costo es infinito
*/
static int costo(Robot robot, Pos2D pos1, Pos2D pos2){
    return (robot->informacion[pos1.y][pos1.x].estado == OBSTACULO || robot->informacion[pos2.y][pos2.x].estado == OBSTACULO) ? inf(robot) : 1;
}

/*
    Devuelve si una posicion esta en el mapa del robot
*/
static int posicion_en_mapa(Robot robot, Pos2D pos){
    return pos.x < robot->laberintoAncho && pos.x >= 0 && pos.y < robot->laberintoAlto && pos.y >= 0;
}

/*
    Obtiene los sucesores de una casilla, osea sus adyacencias.
    Recibe al robot usado, la posicion de la que se quieren calcular las adyacencias, un puntero en el que se almacena la cantidad de sucesores encontrados
    y tambien un puntero para almacenar las direcciones en las que se encontraron sucesores, si este es NULL entonces no se almacenan las direcciones
*/
static Pos2D* obtener_sucesores(Robot robot, Pos2D pos, int *cantSucesores, char *direcciones){
    Pos2D* sucesores = malloc(sizeof(Pos2D) * 4);
    char aux[4];
    if(direcciones == NULL){
        direcciones = aux;
    }

    if(pos.y < robot->laberintoAlto - 1){
        direcciones[(*cantSucesores)] = 'D';
        sucesores[(*cantSucesores)++] = nueva_pos2D(pos.y + 1, pos.x);
    }
    if(pos.y > 0){
        direcciones[(*cantSucesores)] = 'U';
        sucesores[(*cantSucesores)++] = nueva_pos2D(pos.y - 1, pos.x);
    }
    if(pos.x < robot->laberintoAncho - 1){
        direcciones[(*cantSucesores)] = 'R';
        sucesores[(*cantSucesores)++] = nueva_pos2D(pos.y, pos.x + 1);
    }
    if(pos.x > 0){
        direcciones[(*cantSucesores)] = 'L';
        sucesores[(*cantSucesores)++] = nueva_pos2D(pos.y, pos.x - 1);
    }
  
    return sucesores;
}


/*
    Actualiza los valores de una casilla, siguiendo el funcionamiento de la funcion UpdateVertex de D* Lite
*/
static void actualizar_casilla_valores(Robot robot, Pos2D casilla){
    //No actualizamos la salida, esa debe mantenerse en (0, 0)
    if (casilla.x != robot->destino.x || casilla.y != robot->destino.y){
        //rhs en la casilla pasa a ser el minimo (costo(casilla, s') + g(s')) con s' entre los sucesores de la casilla
        int rhs;

        //Si la casilla es un obstaculo obviamos la comparacion entre sucesores pues obviamente sera infinito
        if(robot->informacion[casilla.y][casilla.x].estado == OBSTACULO)
            rhs = inf(robot);
        else{
            int cantSucesores = 0;
            Pos2D* sucesores = obtener_sucesores(robot, casilla, &cantSucesores, NULL);
            rhs = (robot->informacion[sucesores[0].y][sucesores[0].x].g == robot->informacion[sucesores[0].y][sucesores[0].x].rhs ? robot->informacion[sucesores[0].y][sucesores[0].x].g : inf(robot)) + costo(robot, casilla, sucesores[0]);
            for(int i = 1; i <  cantSucesores; i++){
                int nuevoRhs = robot->informacion[sucesores[i].y][sucesores[i].x].g + costo(robot, casilla, sucesores[i]);
                rhs = rhs > nuevoRhs ? nuevoRhs : rhs;
            }
            free(sucesores);
        }
        
        //Uso el operador ternario para evitar valores mayores a infinito, que me causan problemas al definir si una casilla es consistente
        robot->informacion[casilla.y][casilla.x].rhs = rhs > inf(robot) ? inf(robot) : rhs;
    }

    //Una vez actualizado los valores saco mi casilla de la cola
    bheap_eliminar(robot->cola, &casilla);

    //Si sigue inconsistente la volvemos a poner, ahora con sus nuevos valores
    if(robot->informacion[casilla.y][casilla.x].g != robot->informacion[casilla.y][casilla.x].rhs){
        Pos2D llave = calcular_llave(robot, casilla);
        cola_prioridad_insertar(robot->cola, &casilla, &llave);
    }
}

Pos2D robot_obtener_movimiento(Robot robot, char *dirResultante){

    int cantSucesores = 0;
    char sucDirecciones[4];

    //Busco entre mis sucesores mi proximo movimiento
    Pos2D* sucesores = obtener_sucesores(robot, robot->posicion, &cantSucesores, sucDirecciones);
    int resultIndice = 0;
    int resultValor = robot->informacion[sucesores[resultIndice].y][sucesores[resultIndice].x].g + costo(robot, robot->posicion, sucesores[resultIndice]);
    
    //Busco el sucesor con menor g + costo.
    for(int i = 1; i < cantSucesores; i++){
        int sucesorActualValor = robot->informacion[sucesores[i].y][sucesores[i].x].g + costo(robot, robot->posicion, sucesores[i]);

        //Me quedo con la casilla con menor valor, o en caso de empate, una aleatoria entre las 2
        if(resultValor > sucesorActualValor || (resultValor == sucesorActualValor && random_rango(0,1))){
            resultValor = sucesorActualValor;
            resultIndice = i;
        }
    }
    Pos2D result = sucesores[resultIndice];
    free(sucesores);

    //Pasamos tambien la direccion a moverse modificando el puntero recibido
    (*dirResultante) = sucDirecciones[resultIndice];
    return result;
}

void robot_obtener_camino(Robot robot){
    Pos2D llavePosActual = calcular_llave(robot, robot->posicion);
    /*Vamos a continuar este while mientras:
        -La casilla de mi robot sea inconsistente (g != rhs) o
        -Haya llaves mas prioritarias para anlizar en mi cola que las de la casilla del robot
    */
    while(comparar_llaves(((Pos2D*)cola_prioridad_maxima_llave(robot->cola)), &llavePosActual) < 0 || robot->informacion[robot->posicion.y][robot->posicion.x].rhs != robot->informacion[robot->posicion.y][robot->posicion.x].g){

        Pos2D posicion = *(Pos2D*)cola_prioridad_maximo_dato(robot->cola);

        Pos2D llave_vieja = *(Pos2D*)(cola_prioridad_maxima_llave(robot->cola));
        Pos2D llave_nueva = calcular_llave(robot, posicion);

        //Si la llave de la posicion sacada de la cola no coincide con la que deberia, pues es de una llamada anterior de calcular_camino, entonces el valor km no se actualizo
        //Con esto hacemos que valores calculados cuando el robot estaba en otro lado tengan su llave actualizada, y entonces el "cuando" se colocaron en la cola las casillas no afecta la eleccion
        if(comparar_llaves(&llave_vieja, &llave_nueva) < 0){
            //Le actualizamos la llave
            cola_prioridad_actualizar_llave(robot->cola, &posicion, &llave_nueva);
        }
        else{ 
            int cantSucesores = 0;
            Pos2D* sucesores = obtener_sucesores(robot, posicion, &cantSucesores, NULL);

            cola_prioridad_eliminar_maximo(robot->cola);
            //Si el valor real de la casilla es mayor al esperado, lo que implica que podemos mejorar la casilla
            if(robot->informacion[posicion.y][posicion.x].g > robot->informacion[posicion.y][posicion.x].rhs){
                robot->informacion[posicion.y][posicion.x].g = robot->informacion[posicion.y][posicion.x].rhs;

                //Propagamos cambios a sucesores, pues hicimos consistente nuestra casilla
                for(int i = 0; i < cantSucesores; i++)
                    actualizar_casilla_valores(robot, sucesores[i]);

            //Si no eso implica que el valor real es menor al esperado, y entonces ha empeorado asi que le reseteamos el valor real a infinito
            }else{
                robot->informacion[posicion.y][posicion.x].g = inf(robot);
                
                //Actualizamos el valor de nuestra casilla (Lo que la vuelve consistente si no lo era ahora) y propagamos cambios a sucesores
                actualizar_casilla_valores(robot, posicion);
                for(int i = 0; i < cantSucesores; i++)
                    actualizar_casilla_valores(robot, sucesores[i]);
            }
            free(sucesores);
        }

        //Actualizamos la llave del inicio
        llavePosActual = calcular_llave(robot, robot->posicion);
    }
}

Robot robot_inicializar(Pos2D posicion, Pos2D destino, size_t laberintoAlto, size_t laberintoAncho){
    Robot robot = malloc(sizeof(_Robot));
    assert(robot != NULL);

    robot->cola = bheap_crear(laberintoAlto * laberintoAncho + 1, (FuncionComparadora)comparar_llaves, (FuncionComparadora)comparar_posiciones,
    (FuncionDestructora)destruir_pos2D, (FuncionDestructora)destruir_pos2D, 
    (FuncionCopia)copiar_pos2D, (FuncionCopia)copiar_pos2D);
    //El modificador de llave empieza inicializado en 0
    robot->km = 0;
    robot->posicion = posicion;
    robot->destino = destino;
    //Asume que el rango del sensor es 0 al comienzo
    robot->rangoSensor = 1;
    robot->laberintoAlto = laberintoAlto;
    robot->laberintoAncho = laberintoAncho;

    robot->informacion = malloc(sizeof(casillaInfo*) * laberintoAlto);

    //Ponemos el valor g y rhs de todas las casillas en 0 al inicio, excepto el rhs de la salida que es 0 y lo ponemos en la cola
    for(int y = 0; y < laberintoAlto; y++){
        robot->informacion[y] = malloc(sizeof(casillaInfo) * laberintoAncho);

        for(int x = 0; x < laberintoAncho; x++){
            robot->informacion[y][x].rhs = inf(robot);
            robot->informacion[y][x].g = robot->informacion[y][x].rhs;
            robot->informacion[y][x].estado = POR_DESCUBRIR;
            if(y == destino.y && x == destino.x){
                robot->informacion[y][x].rhs = 0;
                robot->informacion[y][x].estado = DESCUBIERTA;
            }
        }
    }

    Pos2D llaveDestino = calcular_llave(robot, robot->destino);
    cola_prioridad_insertar(robot->cola, &robot->destino, &llaveDestino);

    return robot;
}

int robot_llego_a_destino(Robot robot){
    return robot->destino.x == robot->posicion.x && robot->destino.y == robot->posicion.y;
}

void robot_mover_a_posicion(Robot robot, char direccion){
    Pos2D posicionAMoverse;
    posicionAMoverse.y = robot->posicion.y;
    posicionAMoverse.x = robot->posicion.x;

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
            break;
    }

    robot->posicion = posicionAMoverse;
}

void robot_actualizar_casilla_estado(Robot robot, size_t distancia, char direccion){
    int variableX = 0, variableY = 0;
    //Chequeamos en que direccion
    switch(direccion){
     case 'U':
            variableY = -1;
            break;
        case 'D':
            variableY = 1;
            break;
        case 'L':
            variableX = -1;
            break;
        case 'R':
            variableX = 1;
            break;
        default:
            break;
    }

    //Distancias menores al valor del sensor se actualizan a descubiertas
    for(int i = 1; i < distancia; i++){
        robot->informacion[robot->posicion.y + variableY * i][robot->posicion.x + variableX * i].estado = DESCUBIERTA;
    }
    Pos2D pos = nueva_pos2D(robot->posicion.y + variableY * distancia, robot->posicion.x + variableX * distancia);
    //Para evitar salir del mapa
    if(posicion_en_mapa(robot, pos)){
        //Si el sensor da la posicion de un obstaculo
        if(distancia <= robot->rangoSensor){
            robot->informacion[pos.y][pos.x].estado = OBSTACULO;

            //Le actualizamos los valores, pues su costo ha pasado a infinito
            actualizar_casilla_valores(robot, pos);
        }
    }
}

void robot_actualizar_rango_sensor(Robot robot, int direcciones[4]){
    for(int i = 0; i < 4; i++){
        if(robot->rangoSensor < direcciones[i] - 1){
            robot->rangoSensor = direcciones[i] - 1;
        }
    }
}

