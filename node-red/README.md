# node-red

Este directorio contiene el flujo Node-RED del sistema IoT para el monitoreo y control de plantas.

## Funcionalidades

- Asignación dinámica de ID a nuevos dispositivos ESP32.
- Visualización en tiempo real de variables ambientales (temperatura, humedad, luz).
- Control del humidificador en modo manual o automático.
- Ajuste del umbral de humedad directamente desde la UI.
- Visualización con gauges, sliders, botones y gráficas.
- Gestión de visibilidad según dispositivos activos/inactivos.
- Almacenamiento de datos en base de datos MariaDB.
- Preparado para escalar hasta al menos 10 dispositivos fácilmente.

## Requisitos

- Node-RED instalado y corriendo (puede estar en Raspberry Pi o en un servidor).
- MQTT broker (Mosquitto, en este caso instalado en la Raspberry Pi).
- Base de datos MariaDB accesible desde Node-RED.

## Cómo importar el flujo

1. Abre el panel de Node-RED en tu navegador.
2. Haz clic en el menú (☰) > Importar > Desde archivo.
3. Selecciona `flows.json` desde esta carpeta.
4. Ajusta las configuraciones si es necesario (conexión MQTT, base de datos).

## Archivos

- `flows.json`: Flujo completo exportado de Node-RED. Incluye:
  - Subflows para asignación de ID.
  - Controladores para cada dispositivo.
  - Funciones de almacenamiento y visualización.

