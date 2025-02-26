#ifndef DEFINICIONES_H // solo importar si no está definido ya
#define DEFINICIONES_H

// Identificadores de modos/pruebas
#define MODO_LINEA 1
#define MODO_OBSTACULOS 2
#define MODO_FUTBOL 3

// Pines para el sensor siguelíneas de 8 fotodiodos
#define foto0 32
#define foto1 33
#define foto2 25
#define foto3 26
#define foto4 27
#define foto5 14
#define foto6 12
#define foto7 13

// Pines para el sensor de ultrasonidos
#define TRIG 5
#define ECHO 18

// Constante para el cálculo de la distancia
#define velSonido 0.0343 // Velocidad del sonido en cm/μs

// Pines para los motores
#define IN1 2
#define IN2 4
#define IN3 16
#define IN4 17
#define ENA 0
#define ENB 15

// Constantes para el algoritmo de PID
#define KP 25          // Constante proporcional
#define KD 15          // Constante derivativa
#define BASE_SPEED 150 // Velocidad base del robot
#define MAX_SPEED 255  // Velocidad máxima

// Variables globales
extern int modoActual;
extern float distancia;
extern int fotoDiodo[8];

#endif