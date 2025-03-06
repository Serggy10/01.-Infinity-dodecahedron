#include <Adafruit_NeoPixel.h>

#define PIN 12        // Pin al que están conectados los LEDs
#define NUM_LEDS 126  // Número total de LEDs
#define LED_COUNT 12  // Cantidad de LEDs que se encienden gradualmente
#define tiempoEncendidoGradual 100

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

int currentLED = 0;  // LED actual que se está encendiendo
int phase = 1;       // Fase del efecto: 0 (Encendido), 1 (Desplazamiento y apagado)

void setup() {
  strip.begin();
  strip.show();  // Inicializa todos los LEDs apagados
}
void loop() {
  desplazaLEDs(strip.Color(random(256), random(256), random(256)));
    //AllOn(150, 150, 150);
    //delay(1000);
}

void AllOn(int red, int green, int blue) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  strip.show();
}

void desplazaLEDs(uint32_t color) {
  for (int i = 0; i < NUM_LEDS + LED_COUNT; ++i) {
    strip.setPixelColor(i, color);
    //strip.show();
    //if (i >= LED_COUNT) {
    strip.setPixelColor(i - LED_COUNT, strip.Color(0, 0, 0));
    strip.show();
    //}
    delay(tiempoEncendidoGradual);
  }
}
