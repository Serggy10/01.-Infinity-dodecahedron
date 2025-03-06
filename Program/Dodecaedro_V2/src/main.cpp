#include <Arduino.h>

//* V1: Primera prueba con solo función de encendido completo.
//* V2: Optimización del consumo del WiFi. En modo apagado paso de consumo de 150mA a 90mA.
//*     Cambiado nombre a Lampara para que Alexa lo detecte mejor.

// Uncomment the following line to enable serial debug output
#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
#define DEBUG_ESP_PORT Serial
#define NODEBUG_WEBSOCKETS
#define NDEBUG
#endif

#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif

#include <SinricPro.h>
#include <Adafruit_NeoPixel.h>
#include "Lampara.h"
#include "Funciones.h"

#define APP_KEY ""
#define APP_SECRET ""
#define DEVICE_ID ""

#define SSID ""
#define PASS ""

#define BAUD_RATE 9600

Lampara &lampara = SinricPro[DEVICE_ID];

/*************
 * Variables *
 ***********************************************
 * Global variables to store the device states *
 ***********************************************/

// PowerStateController
bool globalPowerState;

// ModeController
std::map<String, String> globalModes;

/*************
 * Callbacks *
 *************/

// PowerStateController
bool onPowerState(const String &deviceId, bool &state)
{
  Serial.printf("[Device: %s]: Powerstate changed to %s\r\n", deviceId.c_str(), state ? "on" : "off");
  globalPowerState = state;
  return true; // request handled properly
}

// ModeController
bool onSetMode(const String &deviceId, const String &instance, String &mode)
{
  Serial.printf("[Device: %s]: Modesetting for \"%s\" set to mode %s\r\n", deviceId.c_str(), instance.c_str(), mode.c_str());
  globalModes[instance] = mode;
  return true;
}

/**********
 * Events *
 *************************************************
 * Examples how to update the server status when *
 * you physically interact with your device or a *
 * sensor reading changes.                       *
 *************************************************/

// PowerStateController
void updatePowerState(bool state)
{
  lampara.sendPowerStateEvent(state);
}

// ModeController
void updateMode(String instance, String mode)
{
  lampara.sendModeEvent(instance, mode, "PHYSICAL_INTERACTION");
}

/*********
 * Setup *
 *********/

void setupSinricPro()
{

  // PowerStateController
  lampara.onPowerState(onPowerState);

  // ModeController
  lampara.onSetMode("modeInstance1", onSetMode);

  SinricPro.onConnected([]
                        { Serial.printf("[SinricPro]: Connected\r\n"); });
  SinricPro.onDisconnected([]
                           { Serial.printf("[SinricPro]: Disconnected\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
};

void setupWiFi()
{
  WiFi.setSleep(true); // Habilita el modo de bajo consumo para el WiFi

  WiFi.begin(SSID, PASS);
  Serial.printf("[WiFi]: Connecting to %s", SSID);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.printf(".");
    delay(250);
  }
  Serial.printf("connected\r\n");
}

void setup()
{
  Serial.begin(BAUD_RATE);
  setupWiFi();
  setupSinricPro();
}

/********
 * Loop *
 ********/
void loop()
{
  SinricPro.handle();
  if (globalPowerState)
  {
    if (globalModes["modeInstance1"] == "Rojo")
      setAllLEDsColor(255, 0, 0);
    if (globalModes["modeInstance1"] == "Azul")
      setAllLEDsColor(0, 0, 255);
    if (globalModes["modeInstance1"] == "Verde")
      setAllLEDsColor(0, 255, 0);
    if (globalModes["modeInstance1"] == "Amarillo")
      setAllLEDsColor(255, 255, 0);
    if (globalModes["modeInstance1"] == "Naranja")
      setAllLEDsColor(255, 165, 0);
    if (globalModes["modeInstance1"] == "Morado")
      setAllLEDsColor(128, 0, 128);
    if (globalModes["modeInstance1"] == "Rosa")
      setAllLEDsColor(255, 192, 203);
    if (globalModes["modeInstance1"] == "Cian")
      setAllLEDsColor(0, 255, 255);
    if (globalModes["modeInstance1"] == "Blanco")
      setAllLEDsColor(255, 255, 255);
    if (globalModes["modeInstance1"] == "Ámbar")
      setAllLEDsColor(255, 191, 0);
    if (globalModes["modeInstance1"] == "Turquesa")
      setAllLEDsColor(64, 224, 208);
    if (globalModes["modeInstance1"] == "Lima")
      setAllLEDsColor(191, 255, 0);
    if (globalModes["modeInstance1"] == "Lavanda")
      setAllLEDsColor(230, 230, 250);
    if (globalModes["modeInstance1"] == "Violeta")
      setAllLEDsColor(238, 130, 238);
    if (globalModes["modeInstance1"] == "Fucsia")
      setAllLEDsColor(255, 0, 255);
    if (globalModes["modeInstance1"] == "Celeste")
      setAllLEDsColor(135, 206, 235);
    if (globalModes["modeInstance1"] == "Magenta")
      setAllLEDsColor(255, 0, 255);
    if (globalModes["modeInstance1"] == "Verde Lima")
      setAllLEDsColor(50, 205, 50);
    if (globalModes["modeInstance1"] == "Oro")
      setAllLEDsColor(255, 215, 0);
    if (globalModes["modeInstance1"] == "Coral")
      setAllLEDsColor(255, 127, 80);
  }
  else
  {
    ApagarDodecaedro();
  }
}