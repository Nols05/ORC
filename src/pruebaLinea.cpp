#include "sensores.h"
#include "motores.h"
#include "definiciones.h"
#include <Arduino.h>

// Variables para el control PID
int posicion = 0;
int error = 0;
int errorAnterior = 0;
int errorAcumulado = 0;
int velocidadIzquierda = 0;
int velocidadDerecha = 0;

void iniciarPruebaLinea()
{
    // Resetear variables de control
    error = 0;
    errorAnterior = 0;
    errorAcumulado = 0;

    // Iniciar con una velocidad moderada para mejor precisión
    adelante(BASE_SPEED);
    delay(100);
}

void ejecutarPruebaLinea()
{
    // Leer sensores
    leerFotodiodoLineas();

    // Calcular posición de la línea
    posicion = calcularPosicion();

    // Calcular error (qué tan lejos está de la línea central)
    error = posicion;

    // Calcular componente derivativa (cambio en el error)
    int errorDiferencial = error - errorAnterior;

    // Calcular componente integral con limitación
    errorAcumulado += error;
    errorAcumulado = constrain(errorAcumulado, -1500, 1500);

    // Resetear acumulado en cambios grandes de dirección
    if ((error > 0 && errorAnterior < 0) || (error < 0 && errorAnterior > 0))
        errorAcumulado = 0;

    // Calcular ajuste de velocidad con énfasis en el componente proporcional
    int ajusteVelocidad = (KP * error / 100) + (KI * errorAcumulado / 100) + (KD * errorDiferencial / 100);

    // Reducir velocidad base en curvas pronunciadas para mayor precisión
    int velocidadBase = BASE_SPEED;
    if (abs(error) > 2000)
    {
        velocidadBase = BASE_SPEED / 2;
    }

    // Calcular velocidades para cada motor
    velocidadIzquierda = velocidadBase - ajusteVelocidad;
    velocidadDerecha = velocidadBase + ajusteVelocidad;

    // Aplicar velocidades a los motores
    motores(velocidadIzquierda, velocidadDerecha);

    // Guardar error actual para la próxima iteración
    errorAnterior = error;
}