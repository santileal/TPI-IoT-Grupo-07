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
