#ifndef _LAMPARA_H_
#define _LAMPARA_H_

#include <SinricProDevice.h>
#include <Capabilities/ModeController.h>
#include <Capabilities/BrightnessController.h>
#include <Adafruit_NeoPixel.h>
#include <Colores.h>

#define LED_PIN 12
#define NUM_LEDS 126 // Changed macro name to avoid conflict
#define LED_COUNT 7
extern std::map<String, String> globalModes;

class Lampara
    : public SinricProDevice,
      public ModeController<Lampara>,
      public BrightnessController<Lampara>
{
  friend class ModeController<Lampara>;
  friend class BrightnessController<Lampara>;

public:
  Lampara(const String &deviceId) : SinricProDevice(deviceId, " Lampara"), strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800){}; // Initialize the strip here
  void SetupLEDs()
  {
    strip.begin();
    strip.clear();
    strip.show();
  }
  void SetColor(String _colorStr);
  void SetModo(String _modo);
  void SetBrillo(uint8_t x);
  void Fill();
  void rainbow(int wait);
  void theaterChaseRainbow(int wait);
  void Latido(int wait);
  void Rayo();
  void Run();
  void Estrellas();
  static void TareaEstrellas(void *pvParameters);
  void EncendidoProgresivo(int pixel);

private:
  Adafruit_NeoPixel strip;
  String colorStr = "";
  uint32_t color = 0x000000;
  uint8_t brillo = 0;
  String modo = "Relleno";
  unsigned long int timer1s = 0;
  bool tareasCreadas = false;
  static const int numeroTareas = 1;
  TaskHandle_t taskHandles[numeroTareas];
};

#endif

void Lampara::SetColor(String _colorStr)
{

  // Serial.println("Color leido desde globalMODES:" + _colorStr);
  if (_colorStr != colorStr)
  {
    colorStr = _colorStr;
    if (colorStr == "Rojo")
      color = ROJO;
    else if (colorStr == "Azul")
      color = AZUL;
    else if (colorStr == "Verde")
      color = VERDE;
    else if (colorStr == "Blanco")
      color = BLANCO;
    else if (colorStr == "Amarillo")
      color = AMARILLO;
    else if (colorStr == "Cyan")
      color = CYAN;
    else if (colorStr == "Magenta")
      color = MAGENTA;
    else if (colorStr == "Naranja")
      color = NARANJA;
    else if (colorStr == "Rosa")
      color = ROSA;
    else if (colorStr == "Violeta")
      color = VIOLETA;
    else if (colorStr == "Lima")
      color = LIMA;
    else if (colorStr == "Plata")
      color = PLATA;
    else if (colorStr == "Oro")
      color = ORO;
    else if (colorStr == "Chocolate")
      color = CHOCOLATE;
    else if (colorStr == "Marron")
      color = MARRON;
    else if (colorStr == "Turquesa")
      color = TURQUESA;
    else if (colorStr == "Púrpura")
      color = PURPURA;
    else if (colorStr == "Ambar")
      color = AMBAR;
    else if (colorStr == "Indigo")
      color = INDIGO;
    else if (colorStr == "Marfil")
      color = MARFIL;
    else if (colorStr == "Coral")
      color = CORAL;
    else if (colorStr == "Salmon")
      color = SALMON;
    else if (colorStr == "Melon")
      color = MELON;
    else if (colorStr == "Lavanda")
      color = LAVANDA;
    else if (colorStr == "Menta")
      color = MENTA;
    else if (colorStr == "Esmeralda")
      color = ESMERALDA;
    else if (colorStr == "Carmesí")
      color = CARMESI;
    else if (colorStr == "Bronce")
      color = BRONCE;
    else if (colorStr == "Negro")
      color = NEGRO;
    else
      Serial.println("Color entrada:" + colorStr);
  }
}

void Lampara::SetModo(String _modo)
{
  if (_modo != "Relleno" && _modo != "Estrellas" && _modo != "Rayo" && _modo != "Arcoiris" && _modo != "Circo" && _modo != "Latido")
    _modo = "Estrellas"; //! "Relleno";

  if (_modo != modo)
  {
    if ((_modo == "Arcoiris" || _modo == "Circo" || _modo == "Latido") && color == NEGRO)
    {
      color = BLANCO;
      colorStr = "Blanco";
      globalModes["modeInstance1"] = "Blanco";
      sendModeEvent("modeInstance1", "Blanco", "PHYSICAL_INTERACTION");
    }
    modo = _modo;
  }
}

void Lampara::Fill()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    strip.setPixelColor(i, color);
  }
  strip.show(); // Muestra los colores en la tira
}

void Lampara::SetBrillo(uint8_t x)
{
  if (x != brillo)
  {
    brillo = x;
    // Fill(0x000000);
    strip.setBrightness(map(brillo, 0, 100, 0, 255));
    // Fill(color);
  }
}

void Lampara::Run()
{
  if (modo == "Relleno")
  {
    if (millis() > timer1s + 1000)
    {
      timer1s = millis();
      Fill();
    }
  }
  else if (modo == "Arcoiris")
  {
    rainbow(10);
  }
  else if (modo == "Rayo")
  {
    Rayo();
  }
  else if (modo == "Estrellas")
  {
    delay(1000);
    Estrellas();
  }
  else if (modo == "Circo")
  {
    theaterChaseRainbow(50);
  }
  else if (modo == "Latido")
  {
    Latido(50);
  }
  else
    Serial.println("Modo: " + modo);
}

void Lampara::rainbow(int wait)
{
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256)
  {
    for (int i = 0; i < strip.numPixels(); i++)
    {
      if (modo != "Arcoiris") // Para salir del for si se cambia el modo y no tener que esperar.
        return;
      else if (color == NEGRO)
      {
        Fill();
        globalModes["modeInstance2"] = "Relleno";
        sendModeEvent("modeInstance2", "Relleno", "PHYSICAL_INTERACTION");
        return;
      }
      else
      {
        int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      }
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

void Lampara::theaterChaseRainbow(int wait)
{
  int firstPixelHue = 0; // First pixel starts at red (hue 0)
  for (int a = 0; a < 30; a++)
  { // Repeat 30 times...
    for (int b = 0; b < 3; b++)
    {                //  'b' counts from 0 to 2...
      strip.clear(); //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for (int c = b; c < strip.numPixels(); c += 3)
      {
        if (modo != "Circo") // Para salir del for si se cambia el modo y no tener que esperar.
          return;
        else if (color == NEGRO)
        {
          Fill();
          globalModes["modeInstance2"] = "Relleno";
          sendModeEvent("modeInstance2", "Relleno", "PHYSICAL_INTERACTION");
          return;
        }
        else
        {
          // hue of pixel 'c' is offset by an amount to make one full
          // revolution of the color wheel (range 65536) along the length
          // of the strip (strip.numPixels() steps):
          int hue = firstPixelHue + c * 65536L / strip.numPixels();
          uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
          strip.setPixelColor(c, color);                       // Set pixel 'c' to value 'color'
        }
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

void Lampara::Latido(int wait)
{
  // Primera pulsación (sístole)
  uint8_t red = (color >> 16) & 0xFF;
  uint8_t green = (color >> 8) & 0xFF;
  uint8_t blue = color & 0xFF;

  for (int brightness = 0; brightness < 255; brightness += 2)
  {
    for (int i = 0; i < NUM_LEDS; ++i)
    {
      if (modo != "Latido") // Para salir del for si se cambia el modo y no tener que esperar.
        return;
      else if (color == NEGRO)
      {
        Fill();
        globalModes["modeInstance2"] = "Relleno";
        sendModeEvent("modeInstance2", "Relleno", "PHYSICAL_INTERACTION");
        return;
      }
      else
      {
        strip.setPixelColor(i, strip.Color(red * brightness / 255, green * brightness / 255, blue * brightness / 255));
      }
    }
    strip.show();
    delay(5); // Ajusta el retardo para controlar la velocidad de subida
  }

  delay(70); // Pausa breve entre sístole y diástole

  for (int brightness = 254; brightness >= 0; brightness -= 2)
  {
    for (int i = 0; i < NUM_LEDS; ++i)
    {
      if (modo != "Latido") // Para salir del for si se cambia el modo y no tener que esperar.
        return;
      else if (color == NEGRO)
      {
        Fill();
        globalModes["modeInstance2"] = "Relleno";
        sendModeEvent("modeInstance2", "Relleno", "PHYSICAL_INTERACTION");
        return;
      }
      else
      {
        strip.setPixelColor(i, strip.Color(red * brightness / 255, green * brightness / 255, blue * brightness / 255));
      }
    }
    strip.show();
    delay(5); // Ajusta el retardo para controlar la velocidad de bajada
  }

  delay(70); // Pausa breve entre sístole y diástole
}

void Lampara::Rayo()
{ // desplazaLEDs(strip.Color(random(256), random(256), random(256)));
  for (int i = 0; i < NUM_LEDS + LED_COUNT; ++i)
  {
    strip.setPixelColor(i, color);
    strip.setPixelColor(i - LED_COUNT, NEGRO);
    strip.show();
    if (modo != "Rayo")
      return;
    delay(10);
  }
}

void Lampara::Estrellas()
{
  if (!tareasCreadas)
  {
    for (int i = 0; i < numeroTareas; ++i)
    {
      int *taskId = new int(i + 1); // Asignar memoria para el identificador de la tarea
      xTaskCreatePinnedToCore(
          TareaEstrellas,  // Función de la tarea
          "Task",          // Nombre de la tarea
          10000,           // Tamaño del stack
          taskId,          // Parámetros de la tarea (identificador de la tarea)
          1,               // Prioridad de la tarea
          &taskHandles[i], // Manejador de la tarea
          1                // Núcleo (ejemplo: núcleo 1)
      );
    }
    tareasCreadas = true; // Marcar que las tareas han sido creadas
  }
}

void Lampara::TareaEstrellas(void *pvParameters)
{
  Lampara *lampara = static_cast<Lampara *>(pvParameters); // Convertir pvParameters a Lampara*
  Serial.println("Tarea Estrellas creada.");

  // Generar un pixel aleatorio dentro del rango válido
  int pixel = random(0, NUM_LEDS - 1);

  lampara->EncendidoProgresivo(pixel);

  vTaskDelete(NULL); // Eliminar la tarea después de completar su ejecución
}

void Lampara::EncendidoProgresivo(int pixel)
{
  // Asegúrate de que strip esté inicializado y configurado correctamente
  strip.begin();
  strip.clear();

  for (int i = 0; i <= 255; ++i)
  {
    strip.setPixelColor(pixel, strip.Color(i, 0, 0));
    strip.show();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}