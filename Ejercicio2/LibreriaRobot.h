#ifndef __LIBRERIAROBOT_H__
    #define __LIBRERIAROBOT_H__
    #include "heaps.h"

    /*
        Las posiciones las uso para mostrar tanto coordenadas como llaves
        En las coordenadas .y da la coordenada y donde a medida que sube se baja en el laberinto
        En las coordenadas .x da la coordenada x donde a medida que sube se va a la derecha en el laberinto

        EN las llaves .y da la primera componente y .x da la segunda (de desempate)
    */
    typedef struct _Pos2D{
        int y;
        int x;
    } Pos2D;

    typedef enum {
        POR_DESCUBRIR,
        DESCUBIERTA,
        OBSTACULO,
    } casillaEstado;

    /*
        g: El costo definido de dicha casilla
        rhs: El costo estimado, basandome en sus adyacencias
    */
    typedef struct _casillaInfo{
        //Rhs es un valor anticipado del g, los cuales actualizamos antes de pasar a cambiar el g
        int rhs;
        //El valor de g representa el coste de moverse a la casilla
        int g;
        casillaEstado estado;
    } casillaInfo;

    typedef struct _R{
        /*
            Informacion es un mapa en el que las llaves son las coordeanadas y el contenido es el estado de las casillas para el robot
        */
        casillaInfo** informacion;
        /*
            Cola que uso para el algoritmo D* Lite. En ella se almacenan las casillas inconsistentes (G != RHS) y se van actualizando al recalcular el camino en
            orden de una llave basada en heuristica, el g y rhs y un modificador de llave.
        */
        BHeap cola;
        Pos2D posicion;
        Pos2D destino;
        //Modificador de llave, se usa para que las llaves de casillas calculadas desde otra posicion no tengan una llave errada al calcular desde otra
        int km;
        //El rango del sensor empieza estimandose que es de 1, conforme el programa corre el robot se hace una idea de la longitud del sensor
        int rangoSensor;
        int laberintoAlto;
        int laberintoAncho;

    } _Robot;

    typedef _Robot* Robot;

    /*
        Devuelve la heuristica entre 2 puntos
    */
    int calcular_heuristica(Pos2D pos1, Pos2D pos2);

    /*
        Devuelve la casilla a la que se desea mover el robot, recibiendo el puntero dirResultante en el que se almacena la direccion que acompaña dicha casilla
    */
    Pos2D robot_obtener_movimiento(Robot robot, char *dirResultante);

    /*
        Calcula el camino a la salida, actualizando los valores de las casillas del mapa del robot.
        Hace uso de la cola del robot para optimizar actualizaciones del camino. (En casos que se deba recalcular gran parte del camino, basicamente se debe actualizar toda la informacion)
    */
    void robot_obtener_camino(Robot robot);

    /*
        Inicializa un robot con su informacion personal sin obstaculos, su posicion inicial y su destino
    */
    Robot robot_inicializar(Pos2D posicion, Pos2D destino, size_t laberintoAlto, size_t laberintoAncho);

    /*
        Devuelve true si el robot llego a su destino, false en caso contrario
    */
    int robot_llego_a_destino(Robot robot);

    /*
        Mueve el robot en la posicion deseada, ('U': Up, 'D': Down, 'R': Right, 'L': Left)
    */
    void robot_mover_a_posicion(Robot robot, char direccion);

    /*
        Actualiza el estado de una casilla para la informacion del robot dado, devuelve 1 si coloco obstaculos, 0 en caso contrario
    */
    void robot_actualizar_casilla_estado(Robot robot, size_t distancia, char direccion);

    /*
        Actualiza el rango del sensor conocido por el robot en caso de encontrar valores mayores
    */
    void robot_actualizar_rango_sensor(Robot robot, int direcciones[4]);

#endif //__LIBRERIAROBOT_H__
