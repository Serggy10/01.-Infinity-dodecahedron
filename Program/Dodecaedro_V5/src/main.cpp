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
#include <Colores.h>

#define APP_KEY ""
#define APP_SECRET ""
#define DEVICE_ID ""

#define SSID ""
#define PASS ""

#define BAUD_RATE 9600

Lampara &lampara = SinricPro[DEVICE_ID];
void TareaSinricPro(void *pvParameters);
void TareaLEDs(void *pvParameters);
/*************
 * Variables *
 ***********************************************
 * Global variables to store the device states *
 ***********************************************/

// ColorController (RGB)
uint8_t globalR = 0, globalG = 0, globalB = 0; // valor por defecto
// PowerStateController
bool globalPowerState;

// ModeController
std::map<String, String> globalModes;

// BrightnessController
int globalBrightness;

/*************
 * Callbacks *
 *************/

// ModeController
bool onSetMode(const String &deviceId, const String &instance, String &mode)
{
  Serial.printf("[Device: %s]: Modesetting for \"%s\" set to mode %s\r\n", deviceId.c_str(), instance.c_str(), mode.c_str());
  globalModes[instance] = mode;
  return true;
}

// BrightnessController
bool onBrightness(const String &deviceId, int &brightness)
{
  Serial.printf("[Device: %s]: Brightness set to %d\r\n", deviceId.c_str(), brightness);
  globalBrightness = brightness;
  return true; // request handled properly
}

bool onAdjustBrightness(const String &deviceId, int &brightnessDelta)
{
  globalBrightness += brightnessDelta; // calculate absolute brigthness
  Serial.printf("[Device: %s]: Brightness changed about %i to %d\r\n", deviceId.c_str(), brightnessDelta, globalBrightness);
  brightnessDelta = globalBrightness; // return absolute brightness
  return true;                        // request handled properly
}

// ColorController
bool onColor(const String &deviceId, byte &r, byte &g, byte &b)
{
  Serial.printf("[Device: %s]: Color set to R:%d G:%d B:%d\r\n", deviceId.c_str(), r, g, b);
  globalR = r;
  globalG = g;
  globalB = b;
  // Si quieres, aquí podrías notificar con sendColorEvent(r,g,b)
  if (globalModes["modeInstance2"] == "Apagado")
    globalModes["modeInstance2"] = "Relleno";
  return true;
}

/**********
 * Events *
 *************************************************
 * Examples how to update the server status when *
 * you physically interact with your device or a *
 * sensor reading changes.                       *
 *************************************************/

// ModeController
void updateMode(String instance, String mode)
{
  lampara.sendModeEvent(instance, mode, "PHYSICAL_INTERACTION");
}

// BrightnessController
void updateBrightness(int brightness)
{
  lampara.sendBrightnessEvent(brightness);
}

/*********
 * Setup *
 *********/

void setupSinricPro()
{

  // ModeController
  lampara.onSetMode("modeInstance1", onSetMode);
  lampara.onSetMode("modeInstance2", onSetMode);

  // BrightnessController
  lampara.onBrightness(onBrightness);
  lampara.onAdjustBrightness(onAdjustBrightness);

  // ColorController (RGB)
  lampara.onColor(onColor);

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
  lampara.SetupLEDs();

  xTaskCreatePinnedToCore(
      TareaSinricPro, // Función de la tarea
      "Core0Task",    // Nombre de la tarea
      10000,          // Tamaño de la pila
      NULL,           // Parámetros de entrada
      1,              // Prioridad de la tarea
      NULL,           // Manejador de la tarea (puede ser NULL si no se necesita)
      0);             // Núcleo en el que se ejecutará la tarea (1 para el segundo núcleo)
  xTaskCreatePinnedToCore(
      TareaLEDs,   // Función de la tarea
      "Core1Task", // Nombre de la tarea
      10000,       // Tamaño de la pila
      NULL,        // Parámetros de entrada
      1,           // Prioridad de la tarea
      NULL,        // Manejador de la tarea (puede ser NULL si no se necesita)
      1);          // Núcleo en el que se ejecutará la tarea (1 para el segundo núcleo)
}

void TareaSinricPro(void *pvParametes)
{
  for (;;)
  {
    SinricPro.handle();
    lampara.SetBrillo(globalBrightness);
    lampara.SetColorRGB(globalR, globalG, globalB);
    // lampara.SetColor(globalModes["modeInstance1"]);
    lampara.SetModo(globalModes["modeInstance2"]);
    delay(10);
  }
}

void TareaLEDs(void *pvParameters)
{
  for (;;)
  {
    lampara.Run();
    delay(10);
  }
}

/********
 * Loop *
 ********/
void loop() {}