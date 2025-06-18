// sensor_handler.cpp
#include "sensor_handler.h"
#include "actuator_handler.h"
#include "mqtt_handler.h"

// Instancia de almacenamiento en NVS
Preferences preferences;
String esp32ID = "";
const long interval = 5000; // Intervalo de lectura de sensores

// Instancia de sensores
Adafruit_SHT31 sht30;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Variables de lectura
float valorTempA = 0;
float valorHumA = 0;
float valorLDR = 0;
float valorTempS = 0;
float valorHumS = 0;

// Variables para control del botón de factory reset
unsigned long buttonPressedTime = 0;
bool buttonActive = false;
bool factoryResetTriggered = false;

// Inicialización y recuperación de configuraciones guardadas en NVS
void setupPreferences() {
  preferences.begin("iot-app", false);
  esp32ID = preferences.getString("device_id", "");

  if (esp32ID == "") {
    esp32ID = "Nuevo";
    preferences.putString("device_id", esp32ID);
    Serial.println("[INICIO] Nuevo ID asignado: " + esp32ID);
  } else {
    Serial.println("[INICIO] ID cargado desde NVS: " + esp32ID);
  }

  // Recuperar configuraciones de control
  mode = preferences.getString("mode", "auto");
  umbral = preferences.getFloat("umbral", 30.0);
  state = preferences.getBool("manual_state", false);

  Serial.println("[NVS] Configuración cargada:");
  Serial.print("  Modo: "); Serial.println(mode);
  Serial.print("  Umbral: "); Serial.println(umbral);
  Serial.print("  Estado manual: "); Serial.println(state ? "ON" : "OFF");
}

// Inicialización de sensores y pines
void setupSensors() {
  if (!sht30.begin(0x44)) {
    Serial.println("Error en el SHT30");
    while(1); // Detiene el programa si falla el sensor
  }
  sensors.begin();
  pinMode(PIN_LDR, INPUT);
  pinMode(FACTORY_RESET_PIN, INPUT_PULLUP);
}

// Lectura de todos los sensores conectados
void readSensors() {
  valorTempA = sht30.readTemperature();
  valorHumA = sht30.readHumidity(); 

  sensors.requestTemperatures(); 
  valorTempS = sensors.getTempCByIndex(0);

  valorLDR = analogRead(PIN_LDR) / 4095.0 * 100.0;
  valorHumS = 100 - analogRead(PIN_HUM_S) / 4095.0 * 100.0;
}

// Construcción del payload en formato JSON con los datos de sensores
String buildSensorPayload() {
  String payload = "{";
  payload += "\"id\":\"" + esp32ID + "\",";
  payload += "\"tempA\":" + String(valorTempA, 2) + ",";
  payload += "\"humA\":" + String(valorHumA, 2) + ",";
  payload += "\"tempS\":" + String(valorTempS, 2) + ",";
  payload += "\"humS\":" + String(valorHumS, 2) + ",";
  payload += "\"ldr\":" + String(valorLDR, 2);
  payload += "}";
  return payload;
}

// Manejo del botón de reset de fábrica
void handleFactoryResetButton() {
  int buttonState = digitalRead(FACTORY_RESET_PIN);

  // Verifica si el botón está presionado (LOW por PULLUP)
  if (buttonState == LOW) {
    if (!buttonActive) {
      buttonActive = true;
      buttonPressedTime = millis();
    }

    if (!factoryResetTriggered && (millis() - buttonPressedTime > RESET_DELAY)) {
      factoryResetTriggered = true;
      resetToFactorySettings();

      delay(1000);
      ESP.restart();
    }
  } else {
    buttonActive = false;
    factoryResetTriggered = false;
  }
}

// Función para reiniciar configuraciones a valores por defecto
void resetToFactorySettings() {
  Serial.println("[FACTORY] Iniciando reset a valores de fábrica...");

  if (mqttClient.connected()) {
    unsubscribeFromPersonalTopics();
  }

  // Restaurar valores por defecto
  mode = "auto";
  umbral = 30.0;
  state = false;
  esp32ID = "Nuevo";

  // Guardar valores en memoria NVS
  preferences.putString("device_id", esp32ID);
  preferences.putString("mode", mode);
  preferences.putFloat("umbral", umbral);
  preferences.putBool("manual_state", state);

  preferences.end();

  Serial.println("[NVS] Valores restaurados a configuración inicial");
  Serial.println("[FACTORY] Reiniciando dispositivo...");
}