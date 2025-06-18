// sensor_handler.h
#ifndef SENSOR_HANDLER_H
#define SENSOR_HANDLER_H

#include <Adafruit_SHT31.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Preferences.h>

// Definición de pines
#define ONE_WIRE_BUS 13
#define PIN_LDR 33
#define PIN_HUM_S 32
#define FACTORY_RESET_PIN 12  // Pin para botón de reset
#define RESET_DELAY 5000      // Tiempo en ms para resetear

// Variables y objetos externos
extern Preferences preferences;
extern String esp32ID;
extern const long interval;

extern Adafruit_SHT31 sht30;
extern DallasTemperature sensors;

// Variables de lectura de sensores
extern float valorTempA;
extern float valorHumA;
extern float valorLDR;
extern float valorTempS;
extern float valorHumS;

// Declaración de funciones
void setupPreferences();
void setupSensors();
void readSensors();
String buildSensorPayload();

void resetToFactorySettings();
void handleFactoryResetButton();

#endif