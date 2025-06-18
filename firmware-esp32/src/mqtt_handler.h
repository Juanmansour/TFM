// mqtt_handler.h
#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <AsyncMqttClient.h>
#include <WiFi.h>
#include <Preferences.h>

// Configuración WiFi y MQTT
extern const char* WIFI_SSID;
extern const char* WIFI_PASSWORD;
extern const IPAddress MQTT_HOST;
extern const int MQTT_PORT;
extern const char* MQTT_USER;
extern const char* MQTT_PASS;

// Variables y funciones externas
extern String esp32ID;
extern TimerHandle_t mqttReconnectTimer;
extern TimerHandle_t wifiReconnectTimer;
extern AsyncMqttClient mqttClient;

// Funciones de configuración y manejo de eventos
void setupMQTT();
void connectToWifi();
void connectToMqtt();
void WiFiEvent(WiFiEvent_t event);
void onMqttConnect(bool sessionPresent);
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
void publishSensorData(const String& payload);

// Funciones para suscripción y desuscripción a tópicos personales
void subscribeToPersonalTopics();
void unsubscribeFromPersonalTopics();

#endif