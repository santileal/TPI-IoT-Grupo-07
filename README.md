# TPI-IoT-Grupo-07
Trabajo Práctico Integrador

Enunciado 

Mediante la plataforma Wokwi o Arduino IDE diseñar sistema embebido basado en ESP32 que monitorea 
el entorno físico mediante el sensor DHT22 y un potenciómetro para la consigna, permitiendo el control 
de actuadores locales (LEDs verde y azul) y la subida de telemetría a la plataforma IoT ThingSpeak. 

1. Configuración de Hardware y Periféricos: 
Conectar el sensor DHT22 al pin GPIO 33, el potenciómetro al pin analógico GPIO 32 (ADC), el LED verde 
al GPIO 23 y el LED azul al GPIO 2, junto con el módulo OLED I²C en los pines estándar (GPIO 21 para SDA 
y GPIO 22 para SCL). 

2. Conectividad Wi-Fi y Bot de Telegram 
Programar el ESP32 para conectarse a una red Wi-Fi local mediante la librería WiFi.h e integrar un bot de 
Telegram con token de acceso para escuchar comandos de control remoto y consulta de estado. 
 
3. Envío Periódico a ThingSpeak 
Implementar la rutina para enviar los valores de temperatura y humedad del DHT22 hacia un canal 
configurado en ThingSpeak utilizando su API Key de escritura mediante solicitudes HTTP. 

4. Comandos del Bot de Telegram:

1.  /start - Configurar el comando /start utilizando un menú clásico de texto enriquecido con 
emojis y formato Markdown/HTML estructurado por secciones, requiriendo que el usuario 
ingrese los comandos mediante texto plano. 

2. /umbral <valor> permite asignar un valor de umbral comprendido entre [1; 1.5]V. El valor por 
defecto es 0.5v. En caso de enviar un valor fuera de rango el sistema asume automáticamente el 
valor original. 

3. /estado - Consulta en tiempo real de temperatura, humedad, voltaje del potenciómetro y estado 
de los LEDs. 

4.  /auto<on/off>- Activa o desactiva el modo de control automático basado en los umbrales del 
potenciómetro (Si la lectura supera el umbral se enciende automáticamente el led azul). El modo 
automático (/autoon) le indica al microcontrolador que tome el valor actual que está leyendo el 
potenciómetro (convertido a voltaje de 0 a 3.3V) y lo utilice como la línea base para disparar los 
actuadores de forma autónoma (luz azul). Si se desactiva (/autooff), el sistema pasa a un modo 
manual donde solo responde a las órdenes directas de Telegram. 

5. /thingup enviar los valores de humedad y temperatura del sensor DHT22 a la nube de  
ThingSpeak
  
7. /display<cmd> - Que permite mostrar el estado de los componentes en el display de la placa 
de desarrollo. Por ejemplo: si se envía desde Telegram /displayled mostrará el estado actual del 
led en la pantalla del módulo OLED.  Es decir, mostrará: “Led encendido/apagado” según 
corresponda. Lo mismo para   el estado del potenciómetro y sensor DTH22. En caso de recibir 
/display+comando no  identificado informar esta situación en la  pantalla. Además de mostrar el 
estado del componente solicitado, debe mantener un búfer visual en las líneas inferiores de la 
pantalla OLED con una lista de los últimos 3 comandos recibidos exitosamente desde el bot de 
Telegram. 

------------------------------------------------------------------------------------------------------------------------------------------------------------------

Condiciones de entrega:

El trabajo práctico deberá presentarse en grupo con un máximo de 6 integrantes por grupo. 
Los grupos deberán informarse mediante el selector de grupos configurado en la UV. 
Se deberá subir a la tarea configurada como 4k01TP02/26 el proyecto Wokwi junto con código 
fuente del sketch (archivo .ino) 

El sketch presentado deberá correr exitosamente sobre la placa de desarrollo UTN provista por la 
cátedra. 
La defensa del trabajo es por grupos pares / impares en las fechas indicadas. 
Fechas de entrega 

Los grupos impares presentan trabajo miércoles 18/11. 

Consideraciones adicionales

Se evaluará la escritura de código legible y bien estructurado. 

Si se detectan proyectos similares, los trabajos quedarán aplazados. 

Se utilizarán los siguientes criterios de evaluación para la corrección del trabajo: 

Criterios de evaluación:
- Estructura del sketch 
- Librerías propias 
- Desarrollo puntos clave 
- Resolución problema mensajes para el display 
- Ejecución sobre placa UTN 
Participación conjunta 
2 / 2 
Muy Bueno

-------------------------------------------------------------------------------

# 🌐 Sistema IoT con ESP32 – Grupo 07

**Trabajo Práctico Integrador – Tecnologías para la Automatización**  
**Universidad Tecnológica Nacional – Facultad Regional Córdoba**

## 📌 Descripción del proyecto

Este proyecto consiste en el desarrollo de un sistema IoT basado en una placa **ESP32**, capaz de adquirir datos del entorno, controlar actuadores y permitir la interacción remota mediante un bot de Telegram.

El sistema integra sensores, actuadores, una pantalla OLED y servicios en la nube para ofrecer monitoreo y control en tiempo real.

El proyecto permite:

- Medir temperatura y humedad mediante un sensor DHT22.
- Leer el voltaje de un potenciómetro mediante una entrada analógica.
- Controlar dos LEDs, uno verde y otro azul.
- Visualizar información y comandos en una pantalla OLED.
- Consultar el estado del sistema y controlar sus funciones mediante Telegram.
- Enviar información de los sensores a ThingSpeak.
- Ejecutar un modo automático de control del LED azul según el voltaje del potenciómetro.

El circuito y el firmware se desarrollan y prueban utilizando **Wokwi**, mientras que la gestión y compilación del código se realizan mediante **PlatformIO**.

---

## 🛠️ Tecnologías y herramientas

| Tecnología | Utilización |
|---|---|
| ESP32 | Microcontrolador principal |
| Arduino Framework | Desarrollo del firmware |
| C++ | Lenguaje de programación |
| PlatformIO | Gestión del proyecto y compilación |
| Wokwi | Simulación del circuito |
| Telegram Bot API | Comunicación y control remoto |
| ThingSpeak | Almacenamiento y visualización de telemetría |
| OLED SSD1306 | Visualización local de información |
| DHT22 | Medición de temperatura y humedad |
| Potenciómetro | Entrada analógica de voltaje |
| Git / GitHub | Control de versiones |

### Librerías utilizadas

Las dependencias se administran desde `platformio.ini`:

- `DHT sensor library for ESPx`: lectura del sensor DHT22.
- `Adafruit Unified Sensor`: soporte para sensores.
- `Adafruit GFX Library`: funciones gráficas.
- `Adafruit SH110X`: soporte para pantallas SH110X.
- `Adafruit SSD1306`: soporte para pantallas SSD1306.
- `UniversalTelegramBot`: comunicación con Telegram.
- `ArduinoJson`: procesamiento de respuestas JSON.
- `ThingSpeak`: comunicación con la plataforma ThingSpeak.

---

## 🔌 Componentes y conexiones

El circuito utiliza una ESP32 DevKit como unidad central.

### Asignación de pines

| Componente | Pin ESP32 | Función |
|---|---|---|
| DHT22 | GPIO 33 | Lectura de datos |
| Potenciómetro | GPIO 32 | Lectura analógica |
| LED verde | GPIO 23 | Salida digital |
| LED azul | GPIO 2 | Salida digital |
| OLED SDA | GPIO 21 | Comunicación I²C |
| OLED SCL | GPIO 22 | Comunicación I²C |

### Componentes adicionales

- 1 ESP32 DevKit V1.
- 1 sensor DHT22.
- 1 potenciómetro.
- 1 LED verde.
- 1 LED azul.
- 2 resistencias de 220 Ω para los LEDs.
- 1 resistencia de 10 kΩ para el pull-up del DHT22.
- 1 pantalla OLED SSD1306 de 128 × 64 píxeles.

### Alimentación y comunicación

- Los componentes se alimentan utilizando los pines de 3.3 V y GND de la ESP32.
- La pantalla OLED utiliza comunicación I²C.
- El DHT22 utiliza una línea de datos con resistencia pull-up de 10 kΩ.
- Cada LED tiene una resistencia de 220 Ω en serie para limitar la corriente.

---

## 🧠 Funcionamiento del sistema

### 1. Lectura de sensores

El sistema obtiene periódicamente:

- **Temperatura:** medida en grados Celsius mediante el DHT22.
- **Humedad:** expresada en porcentaje mediante el DHT22.
- **Voltaje del potenciómetro:** obtenido mediante el ADC de 12 bits de la ESP32.

La conversión del valor analógico a voltaje se realiza considerando una referencia de 3.3 V y un valor máximo de 4095.

### 2. Control de LEDs

El sistema dispone de dos LEDs:

- **LED verde:** indica el estado correspondiente definido para el sistema.
- **LED azul:** puede controlarse manualmente mediante Telegram o automáticamente según el voltaje del potenciómetro.

El control manual del LED azul solamente está disponible cuando el sistema se encuentra en modo manual.

### 3. Modo automático

El modo automático permite controlar el LED azul en función de las variaciones del voltaje del potenciómetro.

Su funcionamiento es el siguiente:

1. Se ejecuta el comando `/autoon`.
2. El sistema toma el voltaje actual del potenciómetro como línea base.
3. Se compara continuamente el voltaje medido con la línea base más el umbral configurado.
4. Si el voltaje supera ese valor, el LED azul se enciende.
5. Cuando el voltaje disminuye por debajo del límite de apagado, el LED azul se apaga.

Se incorpora una histéresis de 0.05 V para evitar cambios repetitivos del estado del LED cuando el voltaje se encuentra cerca del umbral.

**Ejemplo:**

Si la línea base es de 1.63 V y el umbral es de 1.20 V:

- Encendido: al superar 2.83 V.
- Apagado: al descender por debajo de aproximadamente 2.78 V.

### 4. Configuración del umbral

El umbral puede modificarse mediante Telegram.

- Valor predeterminado: 0.5 V.
- Rango permitido: de 1.0 V a 1.5 V.
- Si se ingresa un valor inválido, se informa el error.
- Si el valor está fuera del rango permitido, se restablece el umbral predeterminado.

### 5. Pantalla OLED

La pantalla OLED permite visualizar información del sistema y cambiar entre diferentes vistas mediante comandos de Telegram.

Las vistas disponibles son:

- Estado de los LEDs.
- Voltaje del potenciómetro.
- Temperatura y humedad del DHT22.

Además, mantiene un historial de los últimos tres comandos exitosos recibidos.

Si se recibe un comando de pantalla desconocido, se muestra un mensaje de error en la pantalla.

### 6. Comunicación con Telegram

El sistema se comunica con un bot de Telegram para recibir instrucciones y devolver información.

La comunicación se realiza mediante HTTPS utilizando la biblioteca UniversalTelegramBot y un cliente seguro TLS.

### 7. Integración con ThingSpeak

El sistema envía periódicamente las mediciones de temperatura y humedad a ThingSpeak.

También permite realizar un envío manual mediante Telegram.

Se contempla un intervalo mínimo de 15 segundos entre envíos para respetar las restricciones de la plataforma. La configuración del proyecto establece un intervalo periódico de 20 segundos.

---

## 🤖 Comandos de Telegram

Para comenzar a utilizar el bot, ejecutar `/start`.

| Comando | Descripción |
|---|---|
| `/start` | Muestra la lista de comandos disponibles. |
| `/estado` | Consulta temperatura, humedad, voltaje, modo de funcionamiento, umbral y estado de los LEDs. |
| `/umbral valor` | Configura el umbral de voltaje. |
| `/autoon` | Activa el modo automático y establece la línea base actual. |
| `/autooff` | Desactiva el modo automático y vuelve al modo manual. |
| `/ledazulon` | Enciende manualmente el LED azul. |
| `/ledazuloff` | Apaga manualmente el LED azul. |
| `/thingup` | Envía manualmente las mediciones del DHT22 a ThingSpeak. |
| `/displayled` | Muestra el estado de los LEDs en la pantalla OLED. |
| `/displaypote` | Muestra el voltaje del potenciómetro en la pantalla OLED. |
| `/displaydht` | Muestra temperatura y humedad en la pantalla OLED. |

### Ejemplos de uso

Configurar un umbral de 1.2 V:

```text
/umbral 1.2
```

Activar el modo automático:

```text
/autoon
```

Consultar el estado:

```text
/estado
```

Volver al control manual:

```text
/autooff
```

**Consideraciones:**

- Los comandos de control manual del LED azul no se ejecutan cuando está activo el modo automático.
- El comando `/autoon` establece una nueva línea base cada vez que se ejecuta.
- Los comandos desconocidos reciben una respuesta indicando que no fueron reconocidos.
- Los comandos de pantalla desconocidos también generan un mensaje de error en la OLED.

---

## 📁 Estructura del proyecto

```text
TPI-IoT-Grupo-07/
│
├── platformio.ini
├── wokwi.toml
├── diagram.json
├── README.md
│
├── include/
│   ├── Config.h
│   ├── Secrets.h
│   ├── Sensores.h
│   ├── Actuadores.h
│   ├── Pantalla.h
│   ├── ThingSpeakClient.h
│   └── BotTelegram.h
│
├── src/
│   ├── main.cpp
│   ├── Sensores.cpp
│   ├── Actuadores.cpp
│   ├── Pantalla.cpp
│   ├── ThingSpeakClient.cpp
│   └── BotTelegram.cpp
│
└── lib/
```

### Responsabilidad de los archivos

| Archivo | Responsabilidad |
|---|---|
| `main.cpp` | Inicialización y ciclo principal del sistema. |
| `Config.h` | Pines, parámetros y constantes generales. |
| `Secrets.h` | Credenciales de conexión y servicios externos. |
| `Sensores.h/.cpp` | Inicialización y lectura del DHT22 y potenciómetro. |
| `Actuadores.h/.cpp` | Control de LEDs y lógica automática. |
| `Pantalla.h/.cpp` | Inicialización, visualización e historial OLED. |
| `ThingSpeakClient.h/.cpp` | Comunicación y envío de datos a ThingSpeak. |
| `BotTelegram.h/.cpp` | Recepción, procesamiento y respuesta a comandos. |
| `diagram.json` | Definición del circuito electrónico en Wokwi. |
| `wokwi.toml` | Configuración de la simulación. |
| `platformio.ini` | Configuración de PlatformIO y dependencias. |

---

## ⚙️ Configuración e instalación

### Requisitos previos

Para ejecutar el proyecto se necesita:

- Visual Studio Code.
- Extensión PlatformIO IDE.
- Extensión Wokwi para VS Code.
- Cuenta de Wokwi.
- Cuenta de Telegram.
- Cuenta de ThingSpeak.
- Acceso a una red Wi-Fi o al entorno de simulación Wokwi.

### 1. Clonar el repositorio

```powershell
git clone URL_DEL_REPOSITORIO
```

Ingresar a la carpeta:

```powershell
cd TPI-IoT-Grupo-07
```

Abrir el proyecto desde Visual Studio Code.

### 2. Configurar las credenciales

Crear el archivo `include/Secrets.h` a partir de `Secrets.h.example` y completar los valores correspondientes:

```cpp
#pragma once

#define WIFI_SSID_VALOR      "TU_WIFI"
#define WIFI_PASS_VALOR      "TU_PASSWORD"
#define BOT_TOKEN_VALOR      "TU_TOKEN_TELEGRAM"
#define TS_CHANNEL_ID_VALOR  TU_CHANNEL_ID
#define TS_WRITE_KEY_VALOR   "TU_WRITE_API_KEY"
```

Este archivo contiene información privada y no debe publicarse en GitHub.

**Importante:** para utilizar Wokwi con su red de simulación, se puede configurar:

```cpp
#define WIFI_SSID_VALOR "Wokwi-GUEST"
#define WIFI_PASS_VALOR ""
```

Las credenciales de Telegram y ThingSpeak deben corresponder a los servicios configurados para el proyecto.

### 3. Configurar Telegram

1. Abrir Telegram.
2. Buscar `@BotFather`.
3. Crear un bot mediante `/newbot`.
4. Elegir un nombre y un identificador para el bot.
5. Copiar el token generado.
6. Configurarlo en `Secrets.h`.

No compartir públicamente el token del bot.

### 4. Configurar ThingSpeak

1. Ingresar a [ThingSpeak](https://thingspeak.com/).
2. Crear un canal.
3. Habilitar los campos necesarios para almacenar temperatura y humedad.
4. Obtener el Channel ID.
5. Obtener la Write API Key.
6. Configurar ambos valores en `Secrets.h`.

### 5. Compilar el proyecto

Desde la terminal de PlatformIO:

```powershell
pio run
```

PlatformIO descargará las dependencias necesarias y compilará el firmware.

Si la compilación finaliza correctamente, se generarán los archivos del firmware en:

```text
.pio/build/esp32dev/
```

### 6. Ejecutar la simulación

El proyecto utiliza Wokwi para simular la ESP32 y sus componentes.

Desde Visual Studio Code:

1. Abrir el proyecto.
2. Verificar que `wokwi.toml` apunte al firmware compilado.
3. Ejecutar `Wokwi: Start Simulator` desde la paleta de comandos (`Ctrl + Shift + P`).
4. Observar el funcionamiento del circuito.
5. Abrir la consola serial de Wokwi para consultar los mensajes de diagnóstico.

El archivo `diagram.json` contiene las conexiones de todos los componentes.

---

## 🔧 Parámetros de configuración

Los principales parámetros se encuentran definidos en `Config.h`.

| Parámetro | Valor | Descripción |
|---|---:|---|
| `INTERVALO_TELEGRAM` | 1000 ms | Intervalo nominal entre consultas del bot. |
| `INTERVALO_DHT` | 2000 ms | Intervalo de lectura del DHT22. |
| `INTERVALO_TS` | 20000 ms | Intervalo de envío periódico a ThingSpeak. |
| `INTERVALO_PANTALLA` | 500 ms | Actualización de la pantalla. |
| `INTERVALO_AUTO` | 200 ms | Actualización del control automático. |
| `TS_MIN_ENTRE_ENVIOS` | 15000 ms | Tiempo mínimo entre envíos a ThingSpeak. |
| `UMBRAL_DEFECTO` | 0.5 V | Umbral predeterminado. |
| `UMBRAL_MIN` | 1.0 V | Umbral mínimo configurable. |
| `UMBRAL_MAX` | 1.5 V | Umbral máximo configurable. |
| `HISTERESIS` | 0.05 V | Margen para evitar oscilaciones. |
| `ADC_VREF` | 3.3 V | Referencia de conversión analógica. |
| `ADC_MAX` | 4095 | Resolución máxima utilizada por el ADC. |

Estos valores pueden modificarse según las necesidades del sistema.

---

## 📊 Visualización de datos en ThingSpeak

ThingSpeak permite almacenar y visualizar las mediciones enviadas por la ESP32.

El canal se utiliza para registrar:

- Temperatura del ambiente.
- Humedad relativa.

Los datos pueden consultarse mediante los gráficos y herramientas de visualización que proporciona la plataforma.

[Ingresar a ThingSpeak](https://thingspeak.com/)

---

## 🔐 Seguridad y manejo de credenciales

Las credenciales utilizadas por el sistema se almacenan localmente en `include/Secrets.h`.

Este archivo debe estar incluido en `.gitignore`:

```gitignore
include/Secrets.h
```

El repositorio debe contener un archivo `Secrets.h.example` con valores ficticios para mostrar la estructura necesaria.

**Nunca se deben publicar:**

- Tokens de bots de Telegram.
- Contraseñas de redes Wi-Fi privadas.
- Write API Keys de ThingSpeak.

El cliente HTTPS utiliza `setInsecure()` para facilitar la conexión TLS en el entorno de prueba. Esta configuración no verifica el certificado del servidor y no se recomienda para un despliegue productivo.

---

## 🧪 Pruebas realizadas

Durante el desarrollo se realizaron pruebas funcionales sobre los distintos módulos del sistema:

- Compilación del proyecto mediante PlatformIO.
- Inicialización de la ESP32 en Wokwi.
- Conexión Wi-Fi.
- Lectura del sensor DHT22.
- Lectura del potenciómetro y conversión a voltaje.
- Encendido y apagado manual de los LEDs.
- Activación y desactivación del modo automático.
- Configuración de umbrales dentro y fuera del rango permitido.
- Verificación de la histéresis.
- Consulta del estado mediante Telegram.
- Visualización de datos en la pantalla OLED.
- Registro de comandos recientes.
- Envío de mediciones a ThingSpeak.

Las pruebas se realizan en el entorno de simulación Wokwi.

---

## 🚀 Posibles mejoras futuras

Algunas mejoras que podrían incorporarse en futuras versiones:

- Implementar validación de certificados TLS.
- Incorporar más sensores y actuadores.
- Ampliar las opciones de visualización de datos.
- Incorporar históricos y estadísticas de mediciones.
- Mejorar los mecanismos de reconexión ante fallas de red.
- Optimizar los tiempos de respuesta de la comunicación con Telegram.
- Incorporar nuevos modos de automatización.

---

## 👥 Integrantes

**Grupo 07 – UTN Facultad Regional Córdoba**

Trabajo Práctico Integrador de Tecnologías para la Automatización.

---

## 📄 Licencia

Proyecto desarrollado con fines académicos en el marco de la carrera de Ingeniería en Sistemas de Información de la Universidad Tecnológica Nacional, Facultad Regional Córdoba.