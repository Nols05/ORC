#include "motores.h"
#include <Arduino.h>

void inicializarMotores()
{
    // Configurar pines de los motores
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);

    // Inicialmente detenido
    parar();
}

void adelante(int velocidad)
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, velocidad);
    analogWrite(ENB, velocidad);
}

void atras(int velocidad)
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, velocidad);
    analogWrite(ENB, velocidad);
}

void derecha(int velocidad)
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, velocidad);
    analogWrite(ENB, velocidad);
}

void izquierda(int velocidad)
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, velocidad);
    analogWrite(ENB, velocidad);
}

void parar()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
}

void motores(int velIzquierda, int velDerecha)
{
    // Asegurar que las velocidades están dentro de los límites
    velIzquierda = constrain(velIzquierda, -MAX_SPEED, MAX_SPEED);
    velDerecha = constrain(velDerecha, -MAX_SPEED, MAX_SPEED);

    // Controlar el motor izquierdo
    if (velIzquierda >= 0)
    {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, velIzquierda);
    }
    else
    {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        analogWrite(ENA, -velIzquierda);
    }

    // Controlar el motor derecho
    if (velDerecha >= 0)
    {
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        analogWrite(ENB, velDerecha);
    }
    else
    {
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        analogWrite(ENB, -velDerecha);
    }
}