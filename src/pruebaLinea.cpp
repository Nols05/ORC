#include "sensores.h"
#include "motores.h"
#include "definiciones.h"
#include <Arduino.h>

// Variables para el control PID
int posicion = 0;
int error = 0;
int errorAnterior = 0;
int velocidadIzquierda = 0;
int velocidadDerecha = 0;

void iniciarPruebaLinea()
{
    // Resetear variables de control
    error = 0;
    errorAnterior = 0;

    // Inicialmente, el robot se mueve hacia adelante a velocidad base
    adelante(BASE_SPEED);
    delay(100);
}

void ejecutarPruebaLinea()
{
    // Leer sensores
    leerFotodiodoLineas();
    leerUltrasonidos();

    // Comprobar si hay obstáculo
    if (distancia < 15)
    {
        parar();
        return;
    }

    // Calcular posición de la línea
    posicion = calcularPosicion();

    // Si se ha perdido completamente la línea
    if (posicion == 9999)
    {
        // Estrategia de recuperación: girar hacia donde estaba la línea anteriormente
        if (error > 0)
        {
            derecha(BASE_SPEED);
        }
        else
        {
            izquierda(BASE_SPEED);
        }
        return;
    }

    // Calcular error (qué tan lejos está de la línea central)
    error = posicion;

    // Calcular componente derivativa (cambio en el error)
    int errorDiferencial = error - errorAnterior;

    // Calcular ajuste de velocidad usando PID (solo P y D en este caso)
    int ajusteVelocidad = (KP * error / 100) + (KD * errorDiferencial / 100);

    // Calcular velocidades para cada motor
    velocidadIzquierda = BASE_SPEED - ajusteVelocidad;
    velocidadDerecha = BASE_SPEED + ajusteVelocidad;

    // Aplicar velocidades a los motores
    motores(velocidadIzquierda, velocidadDerecha);

    // Guardar error actual para la próxima iteración
    errorAnterior = error;
}