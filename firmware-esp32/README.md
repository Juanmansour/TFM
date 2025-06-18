

## Funcionalidades principales

- Lectura de sensores:
  - Temperatura y humedad del aire (SHT31)
  - Temperatura del suelo (DS18B20)
  - Humedad del suelo (sensor resistivo)
  - Luminosidad ambiente (LDR)
- Control de un humidificador:
  - Modo automático (dependiendo de la humedad ambiente)
  - Modo manual (encendido/apagado desde la interfaz)
- Publicación de datos y recepción de comandos a través de MQTT
- Almacenamiento persistente de configuración mediante NVS (modo, umbral, estado)
- Reset de fábrica mediante botón físico
- Soporte para suscripción dinámica según el ID asignado

## Requisitos

- Dispositivo: ESP32
- Librerías necesarias:
  - `AsyncMqttClient`
  - `Adafruit_SHT31`
  - `OneWire` + `DallasTemperature`
  - `WiFi` / `Preferences`
- Broker MQTT (probado con Mosquitto en Raspberry Pi 4)
- Node-RED para la interfaz de usuario

## Primeros pasos

1. Clona este repositorio.
2. Abre la carpeta `firmware-esp32/src` con el IDE Arduino o PlatformIO.
3. Instala las dependencias necesarias (ver sección anterior).
4. Compila y sube el archivo `main.ino` al ESP32.
5. El ESP32 buscará conectarse automáticamente al punto de acceso y broker configurados.

## Notas adicionales

- El sistema asigna dinámicamente un ID al ESP32 si no se ha configurado previamente.
- La lógica de control puede personalizarse a través de la interfaz Node-RED.
- En caso de necesitar restaurar a valores de fábrica, mantener presionado el botón físico conectado al pin `12` durante 5 segundos.

## Schematics

Dentro de la carpeta `schematics/` se encuentran los diagramas de conexión de sensores y actuadores al ESP32.

## Archivos del firmware

| Archivo                 | Descripción                                                                 |
|------------------------|-----------------------------------------------------------------------------|
| `TFM.ino`              | Archivo principal del programa. Orquesta la ejecución: inicializa módulos, ejecuta el bucle principal, y coordina sensores, actuadores y MQTT. |
| `mqtt_handler.*`       | Gestiona la conexión a WiFi y al broker MQTT. Publica y recibe datos, maneja reconexiones, y procesa mensajes entrantes. |
| `sensor_handler.*`     | Se encarga de inicializar y leer los sensores conectados al ESP32. También administra las preferencias en NVS y el botón de reseteo de fábrica. |
| `actuator_handler.*`   | Controla el actuador (humidificador) en modo automático o manual. Guarda configuraciones como umbral, modo y estado en memoria NVS. |

---

