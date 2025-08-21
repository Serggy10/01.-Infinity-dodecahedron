#include <Adafruit_NeoPixel.h>



Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void IniciaDodecaedro()
{
    strip.begin();
    strip.clear();
    strip.show();
}

void ApagarDodecaedro()
{
    strip.clear();
    strip.show();
}

void setAllLEDsColor(int red, int green, int blue)
{
    for (int i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, strip.Color(red, green, blue));
    }
    strip.show(); // Muestra los colores en la tira
    Serial.println("setAllLEDsColor");
}
