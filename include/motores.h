#ifndef MOTORES_H
#define MOTORES_H

#include "definiciones.h"

// Inicializar los motores
void inicializarMotores();

// Funciones básicas de movimiento
void adelante(int velocidad);
void atras(int velocidad);
void derecha(int velocidad);
void izquierda(int velocidad);
void parar();

// Control diferencial de motores
void motores(int velIzquierda, int velDerecha);

#endif