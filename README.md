# Documentación del Software de Riego Automatizado

## Descripción General:
El software de riego automatizado para Arduino es un sistema que controla el riego de plantas en función de la humedad del suelo y la intensidad de la luz. Utiliza un Arduino para monitorear sensores, tomar decisiones de riego y comunicarse con dispositivos externos a través de Bluetooth. A continuación se detallan las características y funcionamiento del software.

## Componentes:
- **Arduino Uno**: Controla el sistema, recopila datos de sensores y controla el motor de riego.
- **Sensores**:
  - Sensor de Humedad del Suelo.
  - Sensores de Luz Ambiental.
- **Motor de Riego**: Controla el flujo de agua para el riego de las plantas.
- **Bluetooth**: Permite la comunicación con dispositivos externos para ajustar la configuración y recibir alertas.

## Funcionamiento:

### 1. Inicialización:
- Al encenderse, el Arduino inicializa los sensores, el motor y la comunicación Bluetooth.
- Se establecen los valores iniciales del programa de riego.

### 2. Lectura de Sensores:
- El Arduino lee constantemente los valores de:
  - Humedad del Suelo.
  - Intensidad de Luz (de dos fuentes diferentes).
- Utiliza estos valores para tomar decisiones de riego y generar mensajes de estado.

### 3. Comunicación por Bluetooth:
- El sistema está configurado para recibir comandos de configuración por Bluetooth.
- Los comandos tienen la estructura `CONFIG: frequency,duration,minHumidity,maxHumidity`.
- Al recibir un comando de configuración válido, el Arduino actualiza sus parámetros de riego.

### 4. Decisiones de Riego:
- Basándose en la humedad del suelo y los valores de luz, el sistema decide si es necesario regar.
- Considera la frecuencia de riego y los umbrales de humedad configurados.
- Envía mensajes de estado por Bluetooth indicando la activación o desactivación del motor de riego.

### 5. Alertas y Mensajes:
- Envía alertas por Bluetooth cuando se detectan condiciones anormales, como baja humedad.
- Los mensajes tienen formatos específicos para diferentes estados y alertas.

## Estructura del Código:

### Archivo Principal `main.ino`:
- Incluye las bibliotecas necesarias y define los pines y variables principales.
- Contiene la función `setup()` para la inicialización y `loop()` para el bucle principal.

### Archivos de Funciones `SensorFunctions.h` y `IrrigationSystem.h`:
- Contienen las funciones para el manejo de sensores y control del sistema de riego, respectivamente.
- `SensorFunctions.h`:
  - Inicializa los pines de los sensores.
  - Lee los valores de humedad y luz.
- `IrrigationSystem.h`:
  - Controla el sistema de riego.
  - Verifica las condiciones para activar o desactivar el riego.
  - Envía mensajes por Bluetooth y procesa comandos de configuración.

## Formas de Comunicación por Bluetooth:

El software se comunica a través de Bluetooth y utiliza un protocolo simple de `<TYPE>: <CONTENT>` para intercambiar información y comandos con dispositivos externos.

### 1. Enviar Comandos de Configuración por Bluetooth:
   - **Estructura del Mensaje:**
     - `<TYPE>`: `"CONFIG"`
     - `<CONTENT>`: `"frequency,duration,minHumidity,maxHumidity"`

   - **Ejemplo de Mensaje:**
     - `CONFIG: 15,2,10,80`
   
   - **Significado de los Valores:**
     - `frequency`: Frecuencia de riego en minutos.
     - `duration`: Duración del riego en segundos.
     - `minHumidity`: Humedad mínima del suelo para activar el riego.
     - `maxHumidity`: Humedad máxima del suelo para desactivar el riego.

   - **Uso:**
     - Este mensaje se envía desde un dispositivo externo con capacidad Bluetooth para ajustar la configuración del sistema de riego. Por ejemplo, al enviar `CONFIG: 15,2,10,80`, se actualizarán los valores de frecuencia, duración, humedad mínima y humedad máxima del riego en el Arduino.

### 2. Enviar Mensajes de Datos por Bluetooth:
   - **Estructura del Mensaje:**
     - `<TYPE>`: Puede ser cualquier tipo de mensaje para identificar su contenido.
     - `<CONTENT>`: El contenido específico del mensaje.

   - **Ejemplo de Mensaje:**
     - `HUMEDAD %: 60`
     - `LUZ %: 75`

   - **Uso:**
     - Estos mensajes se utilizan para enviar datos de humedad y luz del suelo al dispositivo externo a través de Bluetooth. Por ejemplo, `HUMEDAD %: 60` indica que la humedad del suelo es del 60%, y `LUZ %: 75` indica que la intensidad de la luz es del 75%.

### 3. Enviar Mensajes de Estado del Motor por Bluetooth:
   - **Estructura del Mensaje:**
     - `<TYPE>`: `"MOTOR ACTIVO"` o `"MOTOR INACTIVO"`
     - `<CONTENT>`: `true` o `false`

   - **Ejemplo de Mensaje:**
     - `MOTOR ACTIVO: true`
     - `MOTOR INACTIVO: false`

   - **Uso:**
     - Estos mensajes se utilizan para indicar si el motor del sistema de riego está activo o inactivo. Por ejemplo, `MOTOR ACTIVO: true` significa que el motor está activo y `MOTOR INACTIVO: false` significa que el motor está inactivo.

### 4. Enviar Mensajes de Configuración Confirmada por Bluetooth:
   - **Estructura del Mensaje:**
     - `<TYPE>`: `"CONFIG ACTUALIZADA"` o `"PARÁMETROS INVÁLIDOS"`
     - `<CONTENT>`: Mensaje adicional para indicar el resultado de la operación.

   - **Ejemplo de Mensaje:**
     - `CONFIG ACTUALIZADA`
     - `PARÁMETROS INVÁLIDOS`

   - **Uso:**
     - Estos mensajes se envían como confirmación después de recibir un mensaje de configuración válido por Bluetooth. Por ejemplo, `CONFIG ACTUALIZADA` confirma que la configuración se ha actualizado correctamente, mientras que `PARÁMETROS INVÁLIDOS` indica que los parámetros de configuración proporcionados son inválidos.

### 5. Enviar Mensajes de Alerta por Bluetooth:
   - **Estructura del Mensaje:**
     - `<TYPE>`: `"ALERTA"`
     - `<CONTENT>`: Mensaje de alerta específico.

   - **Ejemplo de Mensaje:**
     - `ALERTA: ¡Baja humedad detectada!`

   - **Uso:**
     - Estos mensajes se envían como alertas cuando se detecta una condición anormal, como una humedad del suelo por debajo del umbral mínimo. Por ejemplo, `ALERTA: ¡Baja humedad detectada!` alerta sobre una baja humedad del suelo.

### 6. Enviar Mensajes de Configuración Inválida por Bluetooth:
   - **Estructura del Mensaje:**
     - `<TYPE>`: `"FORMATO DE MENSAJE INVÁLIDO"` o `"PARÁMETROS INVÁLIDOS"`
     - `<CONTENT>`: Mensaje adicional para indicar el motivo de la invalidación.

   - **Ejemplo de Mensaje:**
     - `FORMATO DE MENSAJE INVÁLIDO`
     - `PARÁMETROS INVÁLIDOS`

   - **Uso:**
     - Estos mensajes se envían cuando se recibe un mensaje con un formato incorrecto o parámetros inválidos. Por ejemplo, `FORMATO DE MENSAJE INVÁLIDO` indica que el formato del mensaje recibido no es válido, y `PARÁMETROS INVÁLIDOS` indica que los parámetros de configuración son inválidos.