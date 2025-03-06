#include <Arduino.h>
/*
 * Example
 *
 * If you encounter any issues:
 * - check the readme.md at https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md
 * - ensure all dependent libraries are installed
 * - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#arduinoide
 * - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#dependencies
 * - open serial monitor and check whats happening
 * - check full user documentation at https://sinricpro.github.io/esp8266-esp32-sdk
 * - visit https://github.com/sinricpro/esp8266-esp32-sdk/issues and check for existing issues or open a new one
 */

// Custom devices requires SinricPro ESP8266/ESP32 SDK 2.9.6 or later

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
#include "Dodecaedro.h"
#include "Funciones.h"

#define APP_KEY ""
#define APP_SECRET ""
#define DEVICE_ID ""

#define SSID ""
#define PASS ""

#define BAUD_RATE 9600

Dodecaedro &dodecaedro = SinricPro[DEVICE_ID];

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
  dodecaedro.sendPowerStateEvent(state);
}

// ModeController
void updateMode(String instance, String mode)
{
  dodecaedro.sendModeEvent(instance, mode, "PHYSICAL_INTERACTION");
}

/*********
 * Setup *
 *********/

void setupSinricPro()
{

  // PowerStateController
  dodecaedro.onPowerState(onPowerState);

  // ModeController
  dodecaedro.onSetMode("modeInstance1", onSetMode);

  SinricPro.onConnected([]
                        { Serial.printf("[SinricPro]: Connected\r\n"); });
  SinricPro.onDisconnected([]
                           { Serial.printf("[SinricPro]: Disconnected\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
};

void setupWiFi()
{
#if defined(ESP8266)
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.setAutoReconnect(true);
#elif defined(ESP32)
  WiFi.setSleep(false);
  WiFi.setAutoReconnect(true);
#endif

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
float brillo = 0.1;
void loop() {
  SinricPro.handle();
  if (globalPowerState) {
    if (globalModes["modeInstance1"] == "Rojo")
      setAllLEDsColor(255*brillo, 0, 0);
    if (globalModes["modeInstance1"] == "Azul")
      setAllLEDsColor(0, 0, 255*brillo);
    if (globalModes["modeInstance1"] == "Verde")
      setAllLEDsColor(0, 255*brillo, 0);
    if (globalModes["modeInstance1"] == "Amarillo")
      setAllLEDsColor(255*brillo, 255*brillo, 0);
    if (globalModes["modeInstance1"] == "Naranja")
      setAllLEDsColor(255*brillo, 165*brillo, 0);
    if (globalModes["modeInstance1"] == "Morado")
      setAllLEDsColor(128*brillo, 0, 128*brillo);
    if (globalModes["modeInstance1"] == "Rosa")
      setAllLEDsColor(255*brillo, 192*brillo, 203*brillo);
    if (globalModes["modeInstance1"] == "Cian")
      setAllLEDsColor(0, 255*brillo, 255*brillo);
    if (globalModes["modeInstance1"] == "Blanco")
      setAllLEDsColor(255*brillo, 255*brillo, 255*brillo);
    if (globalModes["modeInstance1"] == "Ámbar")
      setAllLEDsColor(255*brillo, 191*brillo, 0);
    if (globalModes["modeInstance1"] == "Turquesa")
      setAllLEDsColor(64*brillo, 224*brillo, 208*brillo);
    if (globalModes["modeInstance1"] == "Lima")
      setAllLEDsColor(191*brillo, 255*brillo, 0);
    if (globalModes["modeInstance1"] == "Lavanda")
      setAllLEDsColor(230*brillo, 230*brillo, 250*brillo);
    if (globalModes["modeInstance1"] == "Violeta")
      setAllLEDsColor(238*brillo, 130*brillo, 238*brillo);
    if (globalModes["modeInstance1"] == "Fucsia")
      setAllLEDsColor(255*brillo, 0, 255*brillo);
    if (globalModes["modeInstance1"] == "Celeste")
      setAllLEDsColor(135*brillo, 206*brillo, 235*brillo);
    if (globalModes["modeInstance1"] == "Magenta")
      setAllLEDsColor(255*brillo, 0, 255*brillo);
    if (globalModes["modeInstance1"] == "Verde Lima")
      setAllLEDsColor(50*brillo, 205*brillo, 50*brillo);
    if (globalModes["modeInstance1"] == "Oro")
      setAllLEDsColor(255*brillo, 215*brillo, 0);
    if (globalModes["modeInstance1"] == "Coral")
      setAllLEDsColor(255*brillo, 127*brillo, 80*brillo);
  } else {
    ApagarDodecaedro();
  }
}