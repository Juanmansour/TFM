// actuator_handler.cpp
#include <Arduino.h>
#include "actuator_handler.h"
#include "sensor_handler.h"

// Inicialización de variables globales
float umbral = 30.0;
String mode = "auto";
bool state = false;

// Configuración inicial de los actuadores
void setupActuators() {
  pinMode(WIFI_LED, OUTPUT);         // LED indicador de conexión WiFi
  pinMode(ACT_LED_HUM, OUTPUT);      // LED/Salida para el humidificador
  digitalWrite(ACT_LED_HUM, LOW);    // Inicialmente apagado
}

// Actualiza el estado del actuador según el modo seleccionado y el valor leído
void updateActuator() {
  if (mode == "auto") {
    // En modo automático se activa si la humedad ambiente es menor al umbral
    digitalWrite(ACT_LED_HUM, (valorHumA < umbral) ? HIGH : LOW);
  } 
  else if (mode == "manual") {
    // En modo manual se activa según el estado definido por el usuario
    digitalWrite(ACT_LED_HUM, state ? HIGH : LOW);
  }
}

// Guarda el modo actual en memoria NVS
void saveModePreference(const String& mode) {
  preferences.putString("mode", mode);
  Serial.println("[NVS] Modo guardado: " + mode);
}

// Guarda el umbral de humedad en NVS
void saveUmbralPreference(float umbral) {
  preferences.putFloat("umbral", umbral);
  Serial.print("[NVS] Umbral guardado: "); Serial.println(umbral);
}

// Guarda el estado manual en NVS
void saveManualStatePreference(bool state) {
  preferences.putBool("manual_state", state);
  Serial.print("[NVS] Estado manual guardado: "); 
  Serial.println(state ? "ON" : "OFF");
}