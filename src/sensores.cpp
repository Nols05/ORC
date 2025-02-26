#include <Arduino.h>
#include "sensores.h"

// Variables globales
float distancia;
int fotoDiodo[8];
int pinesDiodo[] = {foto0, foto1, foto2, foto3, foto4, foto5, foto6, foto7};

void inicializarSensores()
{
    // Configurar pines de entrada y salida para los sensores
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    for (int i = 0; i < 8; i++)
    {
        pinMode(pinesDiodo[i], INPUT);
    }
}

int leerPinDiodo(int n)
{
    return digitalRead(pinesDiodo[n]);
}

void leerFotodiodoLineas()
{
    for (int i = 0; i < 8; ++i)
    {
        fotoDiodo[i] = leerPinDiodo(i);
    }
}

void leerUltrasonidos()
{
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    long duration = pulseIn(ECHO, HIGH);
    distancia = (duration / 2) * velSonido;
}

int calcularPosicion()
{
    int suma = 0;
    int sensoresActivos = 0;

    // Pesos de los sensores: -3500, -2500, -1500, -500, 500, 1500, 2500, 3500
    for (int i = 0; i < 8; i++)
    {
        if (fotoDiodo[i] == 1)
        {
            suma += (i * 1000) - 3500;
            sensoresActivos++;
        }
    }

    if (sensoresActivos == 0)
    {
        // Si no detecta línea, devuelve un valor grande para indicar pérdida de línea
        return 9999;
    }

    return suma / sensoresActivos;
}