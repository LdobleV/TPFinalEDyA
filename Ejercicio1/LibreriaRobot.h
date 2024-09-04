#ifndef __LIBRERIAROBOT_H__
    #define __LIBRERIAROBOT_H__
    #include "LibreriaHash.h"
    #include "pilas.h"

    typedef struct _R{
        /*
            informacion es un mapa en el que las llaves son las coordeanadas y el contenido es el estado de las casillas para el robot
        */
        TablaHash informacion;
        Pos2D posicion;
        Pos2D destino;
        Pila pila;
    } _Robot;

    typedef _Robot* Robot;

    /*
        Arma la informacion del robot
    */
    void armar_robot_informacion(Robot robot, Pos2D posicion);

    /*
        Inicializa un robot con su informacion personal sin obstaculos, su posicion inicial y su destino
    */
    Robot robot_inicializar(Pos2D posicion, Pos2D destino);

    /*
        Devuelve true si el robot llego a su destino, false en caso contrario
    */
    int robot_llego_a_destino(Robot robot);

    /*
        Mueve el robot en la posicion deseada en el laberinto dado, ('U': Up, 'D': Down, 'R': Right, 'L': Left), si la posicion es valida devuelve 1, 0 en caso contrario. Si se ingreso una direccion invalida devuelve -1
    */
    int robot_mover_a_posicion(Robot robot, char direccion, Laberinto laberinto);

    /*
        Actualiza el estado de una casilla adyacente en la direccion dada, para la informacion del robot dado
    */
    void robot_actualizar_casilla(Robot robot, char direccion);

    /*
        Almacena en direccion la mejor direccion a moverse para el robot, devuelve 1 en caso de encontrar direcciones 0 en caso contrario
    */
    int robot_devolver_sig_casilla(Robot robot, char *direccion);

    /*
        Imprime la informacion del robot como si fuese un laberinto
    */
    void robot_imprimir_informacion(Robot robot, Laberinto laberinto);

    /*
        Trata de efectuar un movimiento del robot en el laberinto dado, recibe una pila para efectuar backtracking, devuelve la longitud actual del camino
        Recibe un string con su longitud actual que modifica y va agregando el camino
    */
    size_t robot_elegir_movimiento(Robot robot, Laberinto laberinto, char* camino, size_t caminoLongitud);


#endif //__LIBRERIAROBOT_H__