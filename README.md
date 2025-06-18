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
