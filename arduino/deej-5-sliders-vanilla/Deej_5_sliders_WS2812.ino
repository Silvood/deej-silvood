#include <Adafruit_NeoPixel.h>
#define LED_PIN 4

const int NUM_SLIDERS = 5;
const int analogInputs[NUM_SLIDERS] = {A0, A1, A2, A3, A4};

int analogSliderValues[NUM_SLIDERS];

Adafruit_NeoPixel leds(NUM_SLIDERS, LED_PIN, NEO_GRB + NEO_KHZ800); //define the led type


void setup() { 
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }
  leds.begin();
  leds.clear();
  leds.setBrightness(50); //Change the global brightness of the leds here
  Serial.begin(9600);
}

void loop() {
  updateSliderValues();
  sendSliderValues(); // Actually send data (all the time)
  // printSliderValues(); // For debug
  delay(10);
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
     analogSliderValues[i] = analogRead(analogInputs[i]);

     //Just a simple hysteresis to cancel the noise while having the red color, if you don't, it'll blink.
     if(analogSliderValues[i]<5){ 
      leds.setPixelColor(i,leds.gamma32(leds.ColorHSV(0,255,219)));//red to indicate a mute
     }
     else if(analogSliderValues[i]>15){
      leds.setPixelColor(i,leds.gamma32(leds.ColorHSV(map(analogSliderValues[i],0,1023,8000,57343))));//A map between the slider value and the color wheel of the library     10923 en entrée et 38229 en sortie initialement, mais bleu trop foncé, et jaune pas assez visible
     }
     leds.show();
  }
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }
  
  Serial.println(builtString);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}
