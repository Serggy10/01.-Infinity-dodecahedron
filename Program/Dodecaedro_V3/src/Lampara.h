#ifndef _LAMPARA_H_
#define _LAMPARA_H_

#include <SinricProDevice.h>
#include <Capabilities/ModeController.h>
#include <Capabilities/BrightnessController.h>
#include <Adafruit_NeoPixel.h>
#include <Colores.h>

#define LED_PIN 12
#define NUM_LEDS 126 // Changed macro name to avoid conflict

void ApagarDodecaedro()
{
  // strip.clear();
  // strip.show();
}

void setAllLEDsColor(int red, int green, int blue)
{
  // for (int i = 0; i < strip.numPixels(); i++)
  // {
  //   strip.setPixelColor(i, strip.Color(red, green, blue));
  // }
  // strip.show(); // Muestra los colores en la tira
  // Serial.println("setAllLEDsColor");
}

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
  void Run();

private:
  Adafruit_NeoPixel strip;
  String colorStr = "";
  uint32_t color = 0x000000;
  uint8_t brillo = 0;
  String modo = "Relleno";
  unsigned long int timer1s = 0;
};

#endif

void Lampara::SetColor(String _colorStr)
{
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
  if (_modo != "Relleno" && _modo != "Estrellas" && _modo != "Rayo" && _modo != "Arcoiris")
    _modo = "Relleno";
  if (_modo != modo)
  {
    if (_modo == "Arcoiris" && colorStr == "Negro")
    {
      SetColor("Blanco");
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
    // if (color == NEGRO)
    //   SetColor("Blanco"); // Para que se encienda si o si.
    rainbow(10);
  }
  else if (modo == "Rayo")
  {
  }
  else if (modo == "Estrellas")
  {
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
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      if (modo != "Arcoiris") // Para salir del for si se cambia el modo y no tener que esperar.
        return;
      if (colorStr == "Negro") // Para apagar el dispositivo.
      {
        //sendModeEvent("modeInstance2", "Relleno", "PHYSICAL_INTERACTION");
        // sendModeEvent("modeInstance1", colorStr, "PHYSICAL_INTERACTION");
        Fill();
        return;
      }
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}