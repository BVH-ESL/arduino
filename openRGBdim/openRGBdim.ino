// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            4

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      77

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayValue = 100;
int count = 0;
int delayval = delayValue; // delay for half a second
byte intensity;
bool sign;
//byte i =
int step_rgb = 64;
byte max_rgb = 256;
byte min_rgb = 0;

#define fSensor1 14
#define fSensor2 12
#define rSensor  5

bool isOpen = false;
//byte isPush = 0;
void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  //  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  sign = 1;
  pixels.begin(); // This initializes the NeoPixel library.
  intensity = min_rgb;
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  pixels.show();
  pinMode(fSensor1, INPUT);
  pinMode(fSensor2, INPUT);
  pinMode(rSensor,  INPUT);
  attachInterrupt(rSensor, resetOpen, RISING);
  Serial.begin(115200);
}

void loop() {
  if (isOpen) {
    mode1();
    delay(1000);
//    mode_off();
//    delay(1000);
    mode2();
    delay(1000);
  } else {
    Serial.println(digitalRead(fSensor2));
    if (digitalRead(fSensor1) == HIGH || digitalRead(fSensor2) == HIGH) {
      isOpen = true;
      Serial.println("push");
    }
  }
}

void resetOpen() {
  count = 0;
  isOpen = false;
  mode_off();
}

void mode1() {
  for (int k = 0; k < 2; k++) {
    for (int j = 0; j < 2; j++) {
      int i = 0;
      while (i < NUMPIXELS) {
        if (!isOpen) {
          break;
        }
        if (!sign) {
          //          intensity += step_rgb;
          //          if (intensity == max_rgb) {
          //            intensity = 0;
          //          }
          //        } else {
          intensity -= step_rgb;
          if (intensity == 0) {
            intensity = max_rgb;
          }
        }
        if (count % 2 == 0) {
          pixels.setPixelColor(i, pixels.Color(0, intensity, 0)); // Moderately bright green color.
        } else if (count % 2 == 1) {
          pixels.setPixelColor(i, pixels.Color(0, 0, intensity)); // Moderately bright green color.
        }
        pixels.show();
        if (sign) {
          intensity += step_rgb;
          if (intensity == max_rgb) {
            intensity = 0;
            i++;
            delayval--;
          }
        }
        else {
          //          intensity -= step_rgb;
          if (intensity == 0) {
            //            intensity = max_rgb;
            i++;
            delayval++;
          }
        }
        delay(delayval);
      }
      //      delayval = delayValue;
      sign = !sign;
    }
    count++;
  }
}

void mode2() {
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < NUMPIXELS; i++) {
      if (count % 2 == 0) {
        pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // Moderately bright green color.
      } else if (count % 2 == 1) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 255)); // Moderately bright green color.
      }
      pixels.show();
    }
    delay(200);
    mode_off();
    delay(200);
    count++;
  }
}

void mode_off() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }
}



