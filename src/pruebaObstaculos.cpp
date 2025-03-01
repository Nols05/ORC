#include "sensores.h"
#include "motores.h"
#include "definiciones.h"
#include <Arduino.h>

// Estados para la máquina de estados
enum Estado
{
    SEGUIR_LINEA,
    BUSCAR_NUEVA_LINEA,
    GIRAR_DERECHA,
    GIRAR_IZQUIERDA
};

// Variables para el control
int posicion = 0;
int error = 0;
int errorAnterior = 0;
int velocidadIzquierda = 0;
int velocidadDerecha = 0;
Estado estadoActual = SEGUIR_LINEA;
unsigned long tiempoInicio = 0;
bool direccionGiro = true; // true = derecha, false = izquierda

void iniciarPruebaObstaculos()
{
    // Resetear variables de control
    error = 0;
    errorAnterior = 0;
    estadoActual = SEGUIR_LINEA;

    // Iniciar movimiento
    adelante(BASE_SPEED);
    delay(100);
}

void ejecutarPruebaObstaculos()
{
    // Leer sensores
    leerFotodiodoLineas();
    leerUltrasonidos();

    // Máquina de estados
    switch (estadoActual)
    {

    case SEGUIR_LINEA:
        // Seguir la línea normalmente hasta encontrar un obstáculo
        // Detectar obstáculo
        if (distancia < 15)
        {
            // Encontró un obstáculo, cambiar a estado de búsqueda
            parar();
            delay(200);

            // Alternar la dirección de giro cada vez que encuentra un obstáculo
            direccionGiro = !direccionGiro;

            // Retroceder un poco para tener espacio de maniobra
            atras(BASE_SPEED);
            delay(500);
            parar();
            delay(100);

            if (direccionGiro)
            {
                estadoActual = GIRAR_DERECHA;
            }
            else
            {
                estadoActual = GIRAR_IZQUIERDA;
            }

            tiempoInicio = millis();
            break;
        }

        // Si no hay obstáculo, seguir la línea normalmente con PID
        posicion = calcularPosicion();
        error = posicion;
        int errorDiferencial = error - errorAnterior;
        int ajusteVelocidad = (KP * error / 100) + (KD * errorDiferencial / 100);

        velocidadIzquierda = BASE_SPEED - ajusteVelocidad;
        velocidadDerecha = BASE_SPEED + ajusteVelocidad;
        motores(velocidadIzquierda, velocidadDerecha);

        errorAnterior = error;
        break;

    case GIRAR_DERECHA:
        // Girar a la derecha durante un tiempo
        derecha(BASE_SPEED);

        // Después de cierto tiempo, cambiar a estado de búsqueda
        if (millis() - tiempoInicio > 800)
        {
            estadoActual = BUSCAR_NUEVA_LINEA;
            adelante(BASE_SPEED);
            tiempoInicio = millis();
        }
        break;

    case GIRAR_IZQUIERDA:
        // Girar a la izquierda durante un tiempo
        izquierda(BASE_SPEED);

        // Después de cierto tiempo, cambiar a estado de búsqueda
        if (millis() - tiempoInicio > 800)
        {
            estadoActual = BUSCAR_NUEVA_LINEA;
            adelante(BASE_SPEED);
            tiempoInicio = millis();
        }
        break;

    case BUSCAR_NUEVA_LINEA:
        // Avanzar buscando una nueva línea
        adelante(BASE_SPEED);

        // Verificar si encontró una línea
        leerFotodiodoLineas();
        bool lineaEncontrada = false;
        for (int i = 0; i < 8; i++)
        {
            if (fotoDiodo[i] == 1)
            {
                lineaEncontrada = true;
                break;
            }
        }

        // Si encontró una línea, volver al seguimiento normal
        if (lineaEncontrada)
        {
            estadoActual = SEGUIR_LINEA;
            adelante(BASE_SPEED);
            break;
        }

        // Si ha pasado demasiado tiempo sin encontrar línea, girar en dirección contraria
        if (millis() - tiempoInicio > 2000)
        {
            parar();
            delay(100);
            direccionGiro = !direccionGiro;

            if (direccionGiro)
            {
                estadoActual = GIRAR_DERECHA;
            }
            else
            {
                estadoActual = GIRAR_IZQUIERDA;
            }

            tiempoInicio = millis();
        }
        break;
    }
}