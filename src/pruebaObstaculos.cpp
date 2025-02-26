#include "definiciones.h"
#include "sensores.h"
#include "motores.h"
#include <Arduino.h>

// Estados para la máquina de estados
enum EstadoObstaculos
{
    SEGUIR_LINEA,
    DETECTAR_OBSTACULOS,
    EVITAR_OBSTACULO,
    BUSCAR_LINEA_ALTERNATIVA
};

EstadoObstaculos estadoActual = SEGUIR_LINEA;
unsigned long tiempoEvitando = 0;
int lineaActual = 1; // 0, 1, 2 para las tres líneas

void iniciarPruebaObstaculos()
{
    estadoActual = SEGUIR_LINEA;
    lineaActual = 1;
    adelante(BASE_SPEED);
    delay(100);
}

void ejecutarPruebaObstaculos()
{
    // Leer sensores
    leerFotodiodoLineas();
    leerUltrasonidos();

    // Máquina de estados para la prueba de obstáculos
    switch (estadoActual)
    {
    case SEGUIR_LINEA:
        // Algoritmo similar al de seguimiento de línea
        {
            int posicion = calcularPosicion();
            if (posicion != 9999)
            {
                int error = posicion;
                int ajusteVelocidad = KP * error / 100;
                motores(BASE_SPEED - ajusteVelocidad, BASE_SPEED + ajusteVelocidad);
            }

            // Comprobar si hay obstáculo
            if (distancia < 20)
            {
                estadoActual = DETECTAR_OBSTACULOS;
            }
        }
        break;

    case DETECTAR_OBSTACULOS:
        // Verificar si realmente es un obstáculo que bloquea el camino
        parar();
        delay(100);
        leerUltrasonidos();

        if (distancia < 20)
        {
            // Confirmar obstáculo, decidir evitarlo
            estadoActual = EVITAR_OBSTACULO;
            tiempoEvitando = millis();
        }
        else
        {
            // Falsa alarma, volver a seguir la línea
            estadoActual = SEGUIR_LINEA;
        }
        break;

    case EVITAR_OBSTACULO:
        // Maniobra para evitar el obstáculo
        if (millis() - tiempoEvitando < 500)
        {
            // Retroceder un poco
            atras(100);
        }
        else if (millis() - tiempoEvitando < 1000)
        {
            // Girar para buscar otra línea
            if (lineaActual < 2)
            {
                // Buscar la siguiente línea a la derecha
                derecha(150);
            }
            else
            {
                // Buscar línea a la izquierda si estamos en la última
                izquierda(150);
            }
        }
        else
        {
            // Cambiar a buscar línea alternativa
            estadoActual = BUSCAR_LINEA_ALTERNATIVA;
            adelante(100);
        }
        break;

    case BUSCAR_LINEA_ALTERNATIVA:
        // Buscar una nueva línea
        leerFotodiodoLineas();

        // Verificar si se encontró una línea
        bool lineaEncontrada = false;
        for (int i = 0; i < 8; i++)
        {
            if (fotoDiodo[i] == 1)
            {
                lineaEncontrada = true;
                break;
            }
        }

        if (lineaEncontrada)
        {
            // Actualizar línea actual y volver a seguirla
            if (lineaActual < 2)
            {
                lineaActual++;
            }
            else
            {
                lineaActual = 0;
            }
            estadoActual = SEGUIR_LINEA;
        }
        else
        {
            // Seguir buscando, movimiento en forma de arco
            if (lineaActual < 2)
            {
                motores(80, 150); // Giro derecha suave
            }
            else
            {
                motores(150, 80); // Giro izquierda suave
            }
        }
        break;
    }
}