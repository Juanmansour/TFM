// TFM.ino
#include <Arduino.h>
#include "mqtt_handler.h"
#include "sensor_handler.h"
#include "actuator_handler.h"

// Variable para control de tiempo entre lecturas
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n[INICIO] Iniciando dispositivo ESP32...");

  // Inicialización de preferencias y obtención de ID del dispositivo
  setupPreferences();
  Serial.print("[INICIO] ID actual: ");
  Serial.println(esp32ID);

  // Inicializar sensores, actuadores y comunicación MQTT
  setupSensors();
  setupActuators();
  setupMQTT();

  // Conexión a red WiFi
  connectToWifi();

  // Si ya existe un ID, suscribirse a los tópicos personalizados
  if (esp32ID != "Nuevo" && esp32ID != "") {
    subscribeToPersonalTopics();
  }

  // Imprimir estado inicial del sistema
  Serial.print("[ACTUADOR] Modo inicial: ");
  Serial.println(mode);
  Serial.print("[ACTUADOR] Umbral inicial: ");
  Serial.println(umbral);
}

void loop() {
  unsigned long currentMillis = millis();

  // Verificar si se ha presionado el botón de reset de fábrica
  handleFactoryResetButton();

  // Leer sensores y actuar según sea necesario cada cierto intervalo
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    readSensors(); // Obtener datos de sensores
    String payload = buildSensorPayload(); // Crear JSON con datos
    publishSensorData(payload); // Publicar a través de MQTT

    updateActuator(); // Actualizar estado del actuador según lógica
  }
}
