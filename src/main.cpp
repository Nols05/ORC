#include <Arduino.h>
#include "definiciones.h"

int vel;
float distancia;
int fotoDiodo[8];
int pinesDiodo[] = {foto0, foto1, foto2, foto3, foto4, foto5, foto6, foto7};

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

  long duration = pulseIn(ECHO, HIGH);    // tiempo en microsegundos que tarda una entrada en pasar a HIGH/LOW
  distancia = (duration / 2) * velSonido; // distancia = tiempo · velocidad
}

void adelante(int velocidad)
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
  vel = velocidad;
}

void setup()
{
  Serial.begin(921600);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Hola desde el setup");
}

void loop()
{
  Serial.println("Hola desde el loop");
  digitalWrite(LED_BUILTIN, HIGH);

  delay(1000);

  digitalWrite(LED_BUILTIN, LOW);
  delay(2000);
}
