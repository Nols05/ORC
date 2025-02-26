#ifndef SENSORES_H
#define SENSORES_H

#include "definiciones.h"

// Inicializar los sensores
void inicializarSensores();

// Función para leer un pin de fotodiodo específico
int leerPinDiodo(int n);

// Leer todos los fotodiodos del siguelíneas
void leerFotodiodoLineas();

// Leer sensor de ultrasonidos
void leerUltrasonidos();

// Calcula la posición de la línea basada en los sensores
int calcularPosicion();

#endif