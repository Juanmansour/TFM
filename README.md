# 🌱 Sistema IoT para el Cuidado Inteligente de Plantas

Este proyecto consiste en un sistema IoT distribuido y escalable para el monitoreo y control automatizado del entorno de plantas. Utiliza microcontroladores ESP32 conectados a sensores y actuadores, una interfaz gráfica desarrollada en Node-RED, y un broker MQTT desplegado en una Raspberry Pi 4. Está diseñado para ser fácil de desplegar, portátil y extensible a múltiples plantas.

---

## Componentes del Proyecto

| Carpeta           | Descripción |
|------------------|-------------|
| [`firmware-esp32/`](./firmware-esp32) | Contiene el firmware para los dispositivos ESP32, incluyendo lectura de sensores, control de actuadores y comunicación MQTT. |
| [`node-red/`](./node-red)           | Incluye el flujo de nodos para Node-RED. Proporciona una interfaz gráfica de control y visualización, lógica para asignación de IDs, y conexión a base de datos. |
| [`docs/`](./docs)    | Documentación técnica adicional, esquemas eléctricos, diagramas de arquitectura, etc. |

---

## Arquitectura del Sistema

```text
  +------------------------+           +----------------------------+
  |     Interfaz UI        |<--------->|       Node-RED (RPI)       |
  |  (Dashboard web)       |           |  - Broker MQTT (Mosquitto) |
  +------------------------+           |  - Base de datos MariaDB   |
                                        +----------------------------+
                                                  ^
                                                  |
                                                  v
                                  +-------------------------------+
                                  |           ESP32 x N           |
                                  |-------------------------------|
                                  |  - SHT31 (Temp/Humedad aire)  |
                                  |  - DS18B20 (Temp suelo)       |
                                  |  - Sensor resistivo humedad   |
                                  |  - LDR (Luminosidad)          |
                                  |  - Humidificador (rele/LED)   |
                                  +-------------------------------+
```

## Requisitos

### Hardware

- Raspberry Pi 4  
  - Configurada como punto de acceso WiFi (usando `raspap`)
  - Broker MQTT (Mosquitto)
  - Servidor Node-RED
  - Base de datos MariaDB

- ESP32 con:
  - Sensor de temperatura y humedad del aire (SHT31)
  - Sensor de temperatura del suelo (DS18B20)
  - Sensor resistivo de humedad del suelo
  - Sensor de luz (LDR)
  - Actuador (humidificador o LED/rele)
  - Botón físico para reset de fábrica (opcional)

### Software

- Arduino IDE o PlatformIO (para flashear el firmware en el ESP32)
- Node-RED instalado en Raspberry Pi
- Mosquitto (broker MQTT)
- MariaDB (base de datos relacional)

---

## Primeros pasos

1. **Configura la Raspberry Pi**  
   - Instala Node-RED, Mosquitto y MariaDB.
   - Configura el punto de acceso WiFi utilizando [`raspap`](https://github.com/RaspAP/raspap-webgui).
   - Asegúrate de que el broker MQTT escuche en `10.3.141.1` o la IP correspondiente del AP.

2. **Importa el flujo Node-RED**  
   - Dirígete a [`node-red/`](./node-red/).
   - Sigue las instrucciones del archivo [README](./node-red/README.md).
   - Importa el archivo `flows.json` desde la UI de Node-RED.

3. **Carga el firmware en el ESP32**  
   - Dirígete a [`firmware-esp32/`](./firmware-esp32/).
   - Abre la carpeta `src/` con el IDE Arduino o PlatformIO.
   - Flashea el archivo `TFM.ino` al ESP32.
   - El ESP32 se conectará automáticamente al punto de acceso y comenzará a enviar datos.

4. **Supervisa desde la interfaz**  
   - Accede a la UI de Node-RED desde un navegador (`http://10.3.141.1:1880/ui`).
   - Usa la sección de “Nuevo” para asignar un ID a dispositivos nuevos.
   - Controla el humidificador y visualiza los datos en tiempo real.

---

## Documentación adicional

- [`firmware-esp32/README.md`](./firmware-esp32/README.md): Guía del firmware, estructura de archivos y dependencias.
- [`node-red/README.md`](./node-red/README.md): Descripción del flujo Node-RED, funciones y lógica del dashboard.
- [`docs/`](./docs/): *(Opcional)* Diagramas eléctricos, arquitectura y documentación técnica adicional.


