// mqtt_handler.cpp
#include <Arduino.h>
#include "esp32-hal-gpio.h"
#include "mqtt_handler.h"
#include "sensor_handler.h"
#include "actuator_handler.h"

// Parámetros de red WiFi y MQTT
const char* WIFI_SSID = "Red_IoT";
const char* WIFI_PASSWORD = "iotAP2025";
const IPAddress MQTT_HOST(10, 3, 141, 1);
const int MQTT_PORT = 1883;
const char* MQTT_USER = "usuario_mqtt";
const char* MQTT_PASS = "Tfm$2025,";

// Timers para reconexión WiFi y MQTT
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;
AsyncMqttClient mqttClient;

// Configuración inicial del cliente MQTT y eventos WiFi
void setupMQTT() {
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCredentials(MQTT_USER, MQTT_PASS);
}

// Conectar a la red WiFi
void connectToWifi() {
  if (WiFi.isConnected()) {
    Serial.println("[WIFI] Ya está conectado, no se reconecta.");
    return;
  }
  Serial.println("[WIFI] Conectando a: " + String(WIFI_SSID));
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

// Conectar al broker MQTT
void connectToMqtt() {
  Serial.println("[MQTT] Conectando al broker MQTT...");
  mqttClient.connect();
}

// Manejo de eventos WiFi
void WiFiEvent(WiFiEvent_t event) {
  switch(event) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.println("[WIFI] Conectado!");
      Serial.print("[WIFI] Dirección IP: ");
      Serial.println(WiFi.localIP());
      connectToMqtt();
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("[WIFI] Conexión WiFi perdida"); 
      xTimerStop(mqttReconnectTimer, 0);
      xTimerStart(wifiReconnectTimer, 0);
      break;
  }
}

// Callback al conectarse al broker MQTT
void onMqttConnect(bool sessionPresent) {
  Serial.println("[MQTT] Conectado al broker!");
  digitalWrite(WIFI_LED, HIGH);

  if (esp32ID == "Nuevo" || esp32ID == "") {
    mqttClient.unsubscribe("esp32/id");
    mqttClient.subscribe("esp32/id", 0);
    Serial.println("[MQTT] Suscrito a tópico de asignación de ID");
  } else {
    mqttClient.unsubscribe("esp32/id");
    subscribeToPersonalTopics();
  }
}

// Callback al desconectarse del broker MQTT
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  mqttClient.unsubscribe("esp32/id");

  Serial.println("[MQTT] Conexión MQTT perdida");
  digitalWrite(WIFI_LED, LOW);

  if (WiFi.isConnected()) {
    Serial.println("[MQTT] WiFi activo, reintentando conexión MQTT...");
    xTimerStart(mqttReconnectTimer, 0);
  } else {
    Serial.println("[MQTT] WiFi NO activo, esperando WiFi...");
  }
}

// Manejo de mensajes entrantes desde MQTT
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  String messageTemp(payload, len);
  String topicStr(topic);

  Serial.print("\n[MQTT] Mensaje recibido en tópico: ");
  Serial.println(topic);
  Serial.print("[MQTT] Contenido: ");
  Serial.println(messageTemp);

  bool actuatorNeedsUpdate = false;

  // Asignación de ID al dispositivo
  if (topicStr == "esp32/id") {
    esp32ID = messageTemp;
    preferences.putString("device_id", esp32ID);

    Serial.print("[MQTT] Nuevo ID asignado: ");
    Serial.println(esp32ID);

    subscribeToPersonalTopics();
  }
  // Actualización del modo de funcionamiento
  else if (topicStr.endsWith("/mode_hum")) {
    mode = messageTemp;
    Serial.print("[MQTT] Modo actualizado: ");
    Serial.println(mode);
    saveModePreference(mode);
    actuatorNeedsUpdate = true;
  }
  // Actualización del estado manual
  else if (topicStr.endsWith("/manual_hum")) {
    state = (messageTemp == "true" || messageTemp == "1");
    Serial.print("[MQTT] Control manual: ");
    Serial.println(state ? "ON" : "OFF");
    saveManualStatePreference(state);
    actuatorNeedsUpdate = true;
  }
  // Actualización del umbral de humedad
  else if (topicStr.endsWith("/umbral_hum")) {
    float nuevoUmbral = messageTemp.toFloat();
    if (nuevoUmbral > 0 && nuevoUmbral <= 100) {
      umbral = nuevoUmbral;
      Serial.print("[MQTT] Nuevo umbral humedad: ");
      Serial.println(umbral);
      saveUmbralPreference(umbral);    
      actuatorNeedsUpdate = true;
    }
  }

  // Si alguna configuración cambió, se actualiza el actuador
  if (actuatorNeedsUpdate) {
    updateActuator();
  }
}

// Publicar datos de sensores
void publishSensorData(const String& payload) {
  mqttClient.publish("esp32/sensores", 2, true, payload.c_str());
}

// Suscribirse a los tópicos específicos de este dispositivo
void subscribeToPersonalTopics() {
  if (esp32ID != "Nuevo" && esp32ID != "") {
    String modeTopic = "esp32/" + esp32ID + "/mode_hum";
    String manualTopic = "esp32/" + esp32ID + "/manual_hum";
    String umbralTopic = "esp32/" + esp32ID + "/umbral_hum";

    mqttClient.subscribe(modeTopic.c_str(), 0);
    mqttClient.subscribe(manualTopic.c_str(), 0);
    mqttClient.subscribe(umbralTopic.c_str(), 0);

    Serial.println("[MQTT] Suscrito a tópicos personales:");
    Serial.println("  - " + modeTopic);
    Serial.println("  - " + manualTopic);
    Serial.println("  - " + umbralTopic);
  }
}

// Desuscribirse de los tópicos específicos del dispositivo
void unsubscribeFromPersonalTopics() {
  if (esp32ID != "Nuevo" && esp32ID != "") {
    String modeTopic = "esp32/" + esp32ID + "/mode_hum";
    String manualTopic = "esp32/" + esp32ID + "/manual_hum";
    String umbralTopic = "esp32/" + esp32ID + "/umbral_hum";

    mqttClient.unsubscribe(modeTopic.c_str());
    mqttClient.unsubscribe(manualTopic.c_str());
    mqttClient.unsubscribe(umbralTopic.c_str());

    Serial.println("[MQTT] Desuscrito de tópicos personales");
  }