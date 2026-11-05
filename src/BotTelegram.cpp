#include "BotTelegram.h"
#include "Config.h"
#include <stdlib.h>

BotTelegram::BotTelegram(
  Sensores &sensores,
  Actuadores &actuadores,
  Pantalla &pantalla,
  ThingSpeakClient &thingSpeak
)
  : _bot(BOT_TOKEN, _clienteSeguro),
    _sensores(sensores),
    _actuadores(actuadores),
    _pantalla(pantalla),
    _thingSpeak(thingSpeak) {
}

void BotTelegram::iniciar() {
  // Configuracion TLS para el entorno de prueba.
  _clienteSeguro.setInsecure();

  Serial.println("Bot de Telegram iniciado");
}

void BotTelegram::actualizar() {
  if (WiFi.status() != WL_CONNECTED) return;

  if (millis() - _ultimaConsulta < INTERVALO_TELEGRAM) return;

  _ultimaConsulta = millis();

  int nuevosMensajes = _bot.getUpdates(
    _bot.last_message_received + 1
  );

  // Procesar todos los mensajes pendientes.
  while (nuevosMensajes) {
    for (int i = 0; i < nuevosMensajes; i++) {
      procesarMensaje(i);
    }

    nuevosMensajes = _bot.getUpdates(
      _bot.last_message_received + 1
    );
  }
}

void BotTelegram::enviarMensaje(
  const String &chatId,
  const String &texto
) {
  _bot.sendMessage(chatId, texto, "Markdown");
}

void BotTelegram::registrarComando(const String &comando) {
  _pantalla.registrarComando(comando);
}

void BotTelegram::procesarMensaje(int indice) {
  String chatId = _bot.messages[indice].chat_id;
  String texto = _bot.messages[indice].text;

  texto.trim();

  if (texto.length() == 0) return;

  // Separar el comando de sus argumentos.
  int espacio = texto.indexOf(' ');

  String comando = espacio == -1
    ? texto
    : texto.substring(0, espacio);

  String argumento = espacio == -1
    ? ""
    : texto.substring(espacio + 1);

  comando.toLowerCase();
  argumento.trim();

  // Permitir comandos con mención al bot: /start@NombreBot
  int arroba = comando.indexOf('@');

  if (arroba != -1) {
    comando = comando.substring(0, arroba);
  }

  // -------------------------
  // /start
  // -------------------------
  if (comando == "/start") {
    String ayuda =
      "*Sistema IoT*\n\n"
      "*Comandos disponibles:*\n"
      "/estado - Consultar sensores y LEDs\n"
      "/umbral valor - Configurar umbral\n"
      "/autoon - Activar modo automatico\n"
      "/autooff - Activar modo manual\n"
      "/ledazulon - Encender LED azul manualmente\n"
      "/ledazuloff - Apagar LED azul manualmente\n"
      "/thingup - Enviar datos a ThingSpeak\n"
      "/displayled - Mostrar estado de LEDs\n"
      "/displaypote - Mostrar potenciometro\n"
      "/displaydht - Mostrar datos del DHT22";

    enviarMensaje(chatId, ayuda);
    registrarComando(comando);
    return;
  }

  // -------------------------
  // /estado
  // -------------------------
  if (comando == "/estado") {
    String respuesta = "*Estado del sistema*\n\n";

    if (_sensores.dhtValido()) {
      respuesta += "Temperatura: " +
        String(_sensores.temperatura(), 1) + " C\n";

      respuesta += "Humedad: " +
        String(_sensores.humedad(), 1) + " %\n";
    } else {
      respuesta += "DHT22: sin lectura valida\n";
    }

    respuesta += "Potenciometro: " +
      String(_sensores.leerVoltaje(), 2) + " V\n";

    respuesta += "Modo: ";
    respuesta += _actuadores.modoAuto()
      ? "Automatico\n"
      : "Manual\n";

    respuesta += "Umbral: " +
      String(_actuadores.umbral(), 2) + " V\n";

    respuesta += "LED azul: ";
    respuesta += _actuadores.azulEncendido()
      ? "Encendido\n"
      : "Apagado\n";

    respuesta += "LED verde: ";
    respuesta += _actuadores.verdeEncendido()
      ? "Encendido"
      : "Apagado";

    enviarMensaje(chatId, respuesta);
    registrarComando(comando);
    return;
  }

  // -------------------------
  // /umbral
  // -------------------------
  if (comando == "/umbral") {
    if (argumento.length() == 0) {
      enviarMensaje(
        chatId,
        "Uso: `/umbral valor`\n"
        "El rango permitido es de 1 a 1.5 V."
      );
      return;
    }

    char *fin;
    float valor = strtof(argumento.c_str(), &fin);

    // Verificar que se haya ingresado un numero valido.
    if (fin == argumento.c_str() || *fin != '\0') {
    enviarMensaje(
        chatId,
        "Valor invalido. Ingresa un numero entre 1 y 1.5."
    );
    return;
    }

    if (_actuadores.setUmbral(valor)) {
      enviarMensaje(
        chatId,
        "Umbral actualizado a *" +
        String(_actuadores.umbral(), 2) + " V*."
      );
    } else {
      enviarMensaje(
        chatId,
        "Valor fuera de rango. Se restablecio el umbral "
        "original: *" + String(UMBRAL_DEFECTO, 2) + " V*."
      );
    }

    registrarComando(comando);
    return;
  }

  // -------------------------
  // /autoon
  // -------------------------
  if (comando == "/autoon") {
    float voltajeActual = _sensores.leerVoltaje();

    _actuadores.activarAuto(voltajeActual);

    enviarMensaje(
      chatId,
      "Modo automatico activado.\n"
      "Linea base: *" + String(voltajeActual, 2) + " V*."
    );

    registrarComando(comando);
    return;
  }

  // -------------------------
  // /autooff
  // -------------------------
  if (comando == "/autooff") {
    _actuadores.desactivarAuto();

    enviarMensaje(
      chatId,
      "Modo manual activado. LED azul apagado."
    );

    registrarComando(comando);
    return;
  }

  // -------------------------
  // Control manual del LED azul
  // -------------------------
  if (comando == "/ledazulon" || comando == "/ledazuloff") {

    if (_actuadores.modoAuto()) {
      enviarMensaje(
        chatId,
        "No se puede controlar el LED manualmente "
        "en modo automatico. Usa /autooff primero."
      );
      return;
    }

    bool encender = comando == "/ledazulon";

    _actuadores.setAzul(encender);

    enviarMensaje(
      chatId,
      encender
        ? "LED azul encendido."
        : "LED azul apagado."
    );

    registrarComando(comando);
    return;
  }

  // -------------------------
  // /thingup
  // -------------------------
  if (comando == "/thingup") {

    if (!_sensores.dhtValido()) {
      enviarMensaje(
        chatId,
        "No se puede enviar: el DHT22 no tiene una lectura valida."
      );
      return;
    }

    bool enviado = _thingSpeak.enviar(
      _sensores.temperatura(),
      _sensores.humedad()
    );

    if (enviado) {
      enviarMensaje(
        chatId,
        "Datos enviados correctamente a ThingSpeak."
      );

      registrarComando(comando);
    } else {
      enviarMensaje(
        chatId,
        "No se pudo enviar la informacion. "
        "Verifica la conexion y el intervalo minimo de 15 segundos."
      );
    }

    return;
  }

  // -------------------------
  // Comandos de pantalla
  // -------------------------
  if (comando == "/displayled") {
    _pantalla.setVista(Pantalla::Vista::LED);
    enviarMensaje(chatId, "Mostrando estado de LEDs en pantalla.");
    registrarComando(comando);
    return;
  }

  if (comando == "/displaypote") {
    _pantalla.setVista(Pantalla::Vista::POTE);
    enviarMensaje(chatId, "Mostrando potenciometro en pantalla.");
    registrarComando(comando);
    return;
  }

  if (comando == "/displaydht") {
    _pantalla.setVista(Pantalla::Vista::DHT);
    enviarMensaje(chatId, "Mostrando datos del DHT22 en pantalla.");
    registrarComando(comando);
    return;
  }

  // -------------------------
  // Comando display desconocido
  // -------------------------
  if (comando.startsWith("/display")) {
    _pantalla.mostrarError(comando);

    enviarMensaje(
      chatId,
      "Comando de pantalla desconocido: `" + comando + "`."
    );
    return;
  }

  // -------------------------
  // Comando desconocido
  // -------------------------
  enviarMensaje(
    chatId,
    "Comando no reconocido. Usa /start para ver la lista de comandos."
  );
}