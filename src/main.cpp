#include <Arduino.h>
#include "definiciones.h"

int vel;
float distancia;
int fotoDiodo[8];
int pinesDiodo[] = {foto0, foto1, foto2, foto3, foto4, foto5, foto6, foto7};

// Variables para el control PID
int posicion = 0;
int error = 0;
int errorAnterior = 0;
int velocidadIzquierda = 0;
int velocidadDerecha = 0;

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

// Calcula la posición de la línea basada en los sensores
// Devuelve un valor entre -3500 y 3500, donde:
// 0 = línea centrada, negativo = línea a la izquierda, positivo = línea a la derecha
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
    // Si no detecta línea, mantiene el último valor conocido
    return error > 0 ? 3500 : -3500;
  }

  return suma / sensoresActivos;
}

void adelante(int velocidad)
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
  vel = velocidad;
}

void atras(int velocidad)
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
  vel = velocidad;
}

void derecha(int velocidad)
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
  vel = velocidad;
}

void izquierda(int velocidad)
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
  vel = velocidad;
}

void parar()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  vel = 0;
}

// Función para controlar los motores con diferentes velocidades
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

void setup()
{
  // Inicializar comunicación serial
  Serial.begin(921600);

  // Configurar pines de salida
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Configurar pines de entrada
  pinMode(ECHO, INPUT);
  for (int i = 0; i < 8; i++)
  {
    pinMode(pinesDiodo[i], INPUT);
  }

  // Calibración inicial (opcional)
  delay(1000);
}

void loop()
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

  // Pequeño retardo para estabilidad
  delay(10);
}