// actuator_handler.h
#ifndef ACTUATOR_HANDLER_H
#define ACTUATOR_HANDLER_H

// Definición de pines
#define WIFI_LED 5
#define ACT_LED_HUM 18

// Variables globales de control
extern float umbral;       // Umbral de humedad
extern String mode;        // Modo de funcionamiento: auto o manual
extern bool state;         // Estado manual del humidificador

// Funciones para el actuador
void setupActuators();
void updateActuator();

// Funciones para guardar las preferencias en NVS
void saveModePreference(const String& mode);
void saveUmbralPreference(float umbral);
void saveManualStatePreference(bool state);

#endif