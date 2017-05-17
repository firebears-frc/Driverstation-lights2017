#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PINBOTTOM 3
#define PINTOP 6
#define NUM_PIXELSBOTTOM 122
#define NUM_PIXELSTOP 104
int BackPixelNum = 43; //Number of pixels on the back of the top strip (the part that doesn't correspond to a pixel on the lower strip)

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel stripBottom = Adafruit_NeoPixel(NUM_PIXELSBOTTOM, PINBOTTOM, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripTop = Adafruit_NeoPixel(NUM_PIXELSTOP, PINTOP, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

int Speed = 0;
int Speed2 = 75;
int Wait = 750;

const int colorSwitch = 4;
const int compModeSwitch = 2; //Switch for turning changing lights to blue or red for match play
volatile int colorSensorValue = digitalRead(colorSwitch);

void setup() {
  /* This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
    #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
    #endif
     End of Trinket special code
  */
  pinMode(colorSwitch, INPUT_PULLUP);
  pinMode(compModeSwitch, INPUT_PULLUP);
  //attachInterrupt (digitalPinToInterrupt(blueSwitch), loop, FALLING);
  //attachInterrupt (digitalPinToInterrupt(redSwitch), loop, FALLING);
  attachInterrupt (digitalPinToInterrupt(compModeSwitch), loop, RISING);

  stripBottom.begin();
  stripBottom.show(); // Initialize all pixels to 'off'
  stripBottom.setBrightness(255);
  stripTop.begin();
  stripTop.show(); // Initialize all pixels to 'off'
  stripTop.setBrightness(255);
}

void loop() {
  AllianceSelector();
//TeamNumber();
  FIRST(Speed2);
  Pulse(255);

/*
 
  RainbowPincers();
  colorWipePixRainbow();

  */
  colorWipe(stripBottom.Color(255, 0, 0), Speed); // Red
  delay(Wait);
  colorWipe(stripBottom.Color(0, 0, 0), Speed); //Blank
  colorWipe(stripBottom.Color(0, 0, 255), Speed); // Blue
  delay(Wait);
  colorWipe(stripBottom.Color(0, 0, 0), Speed); //Blank
  colorWipe(stripBottom.Color(200, 46, 0), Speed); //Orange
  delay(Wait);
  colorWipe(stripBottom.Color(0, 0, 0), Speed); //Blank
  colorWipe(stripBottom.Color(0, 225, 0), Speed); //Green
  delay(Wait);
  colorWipe(stripBottom.Color(0, 0, 0), Speed); //Blank

  crossFade(0);
  colorWipe(stripBottom.Color(0, 0, 0), Speed); //Blank
  
  
  colorWipeRainbow(Speed);
  theaterChaseRainbow(Speed2);

  // Send a theater pixel chase in...
  theaterChase(stripBottom.Color(0, 0, 255), Speed2); // Blue
  theaterChase(stripBottom.Color(200, 46, 0), Speed2); // Sweet, sweet, orange
  theaterChase(stripBottom.Color(200, 46, 0), Speed2); // Sweet orange
  theaterChase(stripBottom.Color(0, 255, 0), Speed2); //Green
  theaterChase(stripBottom.Color(200, 46, 0), Speed2); // Sweet, sweet, orange
  theaterChase(stripBottom.Color(200, 46, 0), Speed2); // Sweet orange
  theaterChase(stripBottom.Color(255, 0, 0), Speed2); // Red
  colorWipe(stripBottom.Color(0, 0, 0), Speed);

}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < (stripBottom.numPixels() * 2); i++) {
    stripBottom.setPixelColor(i, c);
    stripTop.setPixelColor(i / 2, c);
    stripBottom.show();
    stripTop.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < stripBottom.numPixels(); i = i + 3) {
        stripBottom.setPixelColor((i + q) * 2, c);  //turn every third pixel on
        stripTop.setPixelColor(i + q, c);
      }
      stripBottom.show();
      stripTop.show();

      delay(wait);

      for (int i = 0; i < stripBottom.numPixels(); i = i + 3) {
        stripBottom.setPixelColor((i + q) * 2, 0);      //turn every third pixel off
        stripTop.setPixelColor(i + q, 0);
      }
    }
  }
}


//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < stripTop.numPixels(); i = i + 3) {
        stripBottom.setPixelColor((i + q) * 2, Wheel(map((i + q) * 2, 0, stripTop.numPixels(), 0, 255))); //turn every third pixel on
        stripTop.setPixelColor(i + q, Wheel(map((i + q), 0, stripTop.numPixels(), 0, 255)));
      }
      stripBottom.show();
      stripTop.show();

      delay(wait);

      for (int i = 0; i < stripBottom.numPixels(); i = i + 3) {
        stripBottom.setPixelColor((i + q) * 2, 0);      //turn every third pixel off
        stripBottom.setPixelColor((i + q), 0);
        stripTop.setPixelColor(i + q, 0);
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return stripBottom.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return stripBottom.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return stripBottom.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


int lowbound = 0;
int highbound = 9;
int slwdwn = 1;
int r = 0;
int g = 0;
int b = 0;

void colorWipeRainbow (uint8_t wait) {
  for (int i = 0; i < stripBottom.numPixels(); i++) {
    stripBottom.setPixelColor(i, Wheel(map(i / 2, 0, stripTop.numPixels(), 0, 255)));
    stripTop.setPixelColor(i, Wheel(map(i, 0, stripTop.numPixels(), 0, 255)));
    stripBottom.show();
    stripTop.show();
    delay(wait);
  } 
  delay(1000);
}
/*
void colorWipePixRainbow () {
  for (int i = 0; i <= stripBottom.numPixels(); i++) {
    for (int c = 0; c < 255; c += 10) {
      setStrips(&stripBottom, &stripTop, i, Wheel(c + i));
      setStrips(&stripBottom, &stripTop, stripBottom.numPixels() - i - 1, Wheel(c + i));
      setStrips(&stripBottom, &stripTop, i + 57, Wheel(c + i));
      setStrips(&stripBottom, &stripTop, 57 - i - 1, Wheel(c + i));
    }
  }
  colorWipe(stripBottom.Color(0, 0, 0), Speed); //Blank
}
*/
void crossFade(uint8_t wait) {
  r = 200;
  g = 55;
  b = 0;

  colorWipe(stripBottom.Color(r, g, b), Speed); //Blank
  for (int r = 200; r > 0 && g < 255; r-- && g++) {
    for (int i = 0; i < stripBottom.numPixels(); i++) {
      stripBottom.setPixelColor(i, r, g, b); //fades to green
      stripTop.setPixelColor(i, r, g, b);
    }
    stripBottom.show();
    stripTop.show();
  }
  delay(250);
  r = 0;
  g = 255;
  for (int b = 0; b < 255; b++) {
    for (int i = 0; i < stripBottom.numPixels(); i++) {
      stripBottom.setPixelColor(i, 0, g, b); //fades to blue green
      stripTop.setPixelColor(i, r, g, b);
    }
    stripBottom.show();
    stripTop.show();
  }
  b = 255;
  r = 0;
  for (int g = 255; g > 0; g--) {
    for (int i = 0; i < stripBottom.numPixels(); i++) {
      stripBottom.setPixelColor(i, r, g, b); //fades green back to only blue
      stripTop.setPixelColor(i, r, g, b);
    }
    stripBottom.show();
    stripTop.show();
  }
  g = 0;
  b = 255;
  for (int r = 0; r < 255; r++) {
    for (int i = 0; i < stripBottom.numPixels(); i++) {
      stripBottom.setPixelColor(i, r, g, b); //fades to purple
      stripTop.setPixelColor(i, r, g, b);
    }
    stripBottom.show();
    stripTop.show();
  }
  r = 255;
  for (int b = 255; b > 0; b--) {
    for (int i = 0; i < stripBottom.numPixels(); i++) {
      stripBottom.setPixelColor(i, r, g, b); //fades blue back to only red
      stripTop.setPixelColor(i, r, g, b);
    }
    stripBottom.show();
    stripTop.show();
  }
  r = 255;
  b = 0;
  for (int g = 0; g < 55 && r > 200; g++ && r--) {
    for (int i = 0; i < stripBottom.numPixels(); i++) {
      stripBottom.setPixelColor(i, r, g, b); //fades to orange
      stripTop.setPixelColor(i, r, g, b);
    }
    stripBottom.show();
    stripTop.show();
  }
 // delay(500);
 // colorWipe(stripBottom.Color(0, 0, 0), Speed); //Blank
}


void Red() {
  while (digitalRead(colorSwitch) == HIGH  && digitalRead(compModeSwitch) == HIGH) {
    colorWipe(stripBottom.Color(100, 0, 0), Speed);
  }
  colorWipe(stripBottom.Color(0, 0, 0), Speed); //Blank
  loop();
}

void Blue() {
  while (digitalRead(colorSwitch) == LOW && digitalRead(compModeSwitch) == HIGH) {
    colorWipe(stripBottom.Color(0, 0, 100), Speed);
  }
  colorWipe(stripBottom.Color(0, 0, 0), Speed); //Blank
  loop();
}

void AllianceSelector() {
  if (digitalRead(compModeSwitch) == HIGH) {    
    if (digitalRead(colorSwitch) == LOW) {
      Blue();
    }
    if (digitalRead(colorSwitch) == HIGH) {
     Red();
    }
  }
}

void FireStart() {
  for (int i = 0; i < 2 * stripTop.numPixels(); i++) {
    r = rand() % 106 + 150;
    g = rand() % 125;
    int      k = rand() % i;
    stripBottom.setPixelColor(i, r, g, b);
    stripBottom.setPixelColor(k, r, g, b);
    r = rand() % 106 + 150;
    g = rand() % 125;
    k = rand() % i;
    stripTop.setPixelColor(i / 2, r, g, b);
    stripTop.setPixelColor(k / 2, r, g, b);
    stripBottom.show();
    stripTop.show();
    delay(NUM_PIXELSBOTTOM/3);
  }
}
void FireContinue() {
  for (int j = 0; j < 2500; j++) {
    r = rand() % 106 + 150;
    g = rand() % 125;
    int   i = rand() % stripBottom.numPixels() ;
    stripBottom.setPixelColor(i, r, g, b);
    stripBottom.show();
    stripTop.setPixelColor(i, r, g, b);
    stripTop.show();
    delay(NUM_PIXELSBOTTOM/3);
  }
}
void FireEnd() {
  for (int i = 129; i > 0; i--) {
    r = rand() % 106 + 150;
    g = rand() % 125;
    int    k = rand() % i;
    stripBottom.setPixelColor(i, 0, 0, 0); //Blanks out the pixels in a colorWipe while the remaining ones still flicker
    stripBottom.setPixelColor(k, r, g, b); //Remaining pixels
    r = rand() % 106 + 150;
    g = rand() % 125;
    k = rand() % i;
    stripTop.setPixelColor(119 - i, 0, 0, 0); //Same as above, just with stripTop
    stripTop.setPixelColor(119 - k, r, g, b);
    stripBottom.show();
    stripTop.show();
    delay(NUM_PIXELSBOTTOM/3);
  }
  delay(100);
}
void Fire() {
  FireStart();
  FireContinue();
  FireEnd();
}


void Pulse(uint32_t c) {
  for (int j = 0; j < 4; j++) { //does 4 cycles of pulses
    for (int i = 0; i < (stripTop.numPixels() * 2) + 8; i++) {
      for (int e = 0; e <= 4; e++) {
        stripBottom.setPixelColor(i - e, 0, c / (int)(pow(2, e)), 0);
        stripTop.setPixelColor((i - e) / 2, 0, c / (int)(pow(2, e)), 0);
      }
      stripBottom.setPixelColor(i - 5, 0);
      stripTop.setPixelColor((i - 5) / 2, 0);
      stripBottom.show();
      stripTop.show();
      delay(3);
    }
  }
}

void FIRST(uint8_t wait) {
  for (int i = 0; i < stripTop.numPixels() * 2; i++) {
    if (i <= stripBottom.numPixels() / 3) {
      stripBottom.setPixelColor(i, 255, 0, 0);
    }
    if (i > stripBottom.numPixels() / 3 && i <= 2 * stripBottom.numPixels() / 3) {
      stripBottom.setPixelColor(i, 255, 255, 255);
    }
    if (i > 2 * stripBottom.numPixels() / 3 && i <= stripBottom.numPixels()) {
      stripBottom.setPixelColor(i, 0, 0, 255);
    }
    //Beginning of top sections
  
    if (i <= (2 * (stripTop.numPixels() - BackPixelNum)) / 3) {      //Pixels 0 - 20 Red
      stripTop.setPixelColor(i / 2, 255, 0, 0);
    }
    if (i > (2 * ((stripTop.numPixels() - BackPixelNum) / 3)) && i <= 4 * ((stripTop.numPixels() - BackPixelNum) / 3)) {     //Pixels 21 - 40 White
      stripTop.setPixelColor(i / 2, 255, 255, 255);
    }
    if (i > (4 * ((stripTop.numPixels() - BackPixelNum) / 3)) && i <= 2 * (stripTop.numPixels() - 35)) {      //Pixels 41 - 69 Blue
      stripTop.setPixelColor(i / 2, 0, 0, 255);
    }
    if (i > (2 * (stripTop.numPixels() - 35)) && i <= 2 * (stripTop.numPixels() - 12)) {      //Pixels 70 - 92 White
      stripTop.setPixelColor(i / 2, 255, 255, 255);
    }
    if (i > 2 * (stripTop.numPixels() - 12)) {      //Pixels 93 - 104 Red
      stripTop.setPixelColor(i / 2, 255, 0, 0);
    }
    delay(10);
    stripBottom.show();
    stripTop.show();
  }
  for (int j = 0; j < 100; j++) { //do 100 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < stripBottom.numPixels(); i = i + 3) {
        if (i < stripBottom.numPixels() / 3) {
          stripBottom.setPixelColor(i + q, stripBottom.Color(255, 0, 0));
        }
        if (i > (2 * (stripBottom.numPixels() - 1) / 3)) {
          stripBottom.setPixelColor(i + q, stripBottom.Color(0, 0, 255));
        }
        if (i > (stripBottom.numPixels() / 3) && i < 2 * stripBottom.numPixels() / 3) {
          stripBottom.setPixelColor(i + q, stripBottom.Color(85, 85, 85));
        }
        
        //Start of top strip
        if (i <= (stripTop.numPixels() - BackPixelNum) / 3) {      //Pixels 0 - 20 Red
          stripTop.setPixelColor(i + q, stripTop.Color(255, 0, 0));
        }
        if (i > ((stripTop.numPixels() - BackPixelNum) / 3) && i <= 2 * (stripTop.numPixels() - BackPixelNum) / 3) {     //Pixels 21 -40 White
          stripTop.setPixelColor(i + q, stripTop.Color(85, 85, 85));
        }
        if (i > (2 * (stripTop.numPixels() - BackPixelNum) / 3) && i <= (stripTop.numPixels() - 35)) {      //Pixels 41 - 69 Blue
          stripTop.setPixelColor(i + q, stripTop.Color(0, 0, 255));
        }
        if (i > (stripTop.numPixels() - 35) && i <= (stripTop.numPixels() - 12)) {      //Pixels 70 - 92 White
          stripTop.setPixelColor(i + q, stripTop.Color(85, 85, 85));
        }
        if (i > stripTop.numPixels() - 12) {      //Pixels 93 -104 Red
          stripTop.setPixelColor(i + q, stripTop.Color(255, 0, 0));
        }
      }
      stripBottom.show();
      stripTop.show();

      delay(wait);

      for (int i = 0; i < stripBottom.numPixels(); i = i + 3) {
        stripBottom.setPixelColor(i + q, 0);      //turn every third pixel off
        stripTop.setPixelColor(i + q, 0);
      }
    }
  }
  for (int i = 0; i < stripBottom.numPixels(); i++) {
    stripBottom.setPixelColor(i, 0, 0, 0);
    stripTop.setPixelColor(i, 0, 0, 0);
  }
  stripBottom.show();
  stripTop.show();
}
/*
void Heartbeat() {
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < stripBottom.numPixels(); i++) {
      stripBottom.setPixelColor(i, 25, 0, 0);
      stripTop.setPixelColor(i, 25, 0, 0);
    }
    stripBottom.show();
    stripTop.show();
    delay(1000);
    for (int i = 0; i < stripBottom.numPixels(); i++) {
      stripBottom.setPixelColor(i, 180, 0, 0);
      stripTop.setPixelColor(i, 180, 0, 0);
    }
    stripBottom.show();
    stripTop.show();
    delay(50);
    for (int i = 0; i < stripBottom.numPixels(); i++) {
      stripBottom.setPixelColor(i, 25, 0, 0);
      stripTop.setPixelColor(i, 25, 0, 0);
    }
    stripBottom.show();
    stripTop.show();
    delay(75);
    for (int i = 0; i < stripBottom.numPixels(); i++) {
      stripBottom.setPixelColor(i, 180, 0, 0);
      stripTop.setPixelColor(i, 180, 0, 0);
    }
    stripBottom.show();
    stripTop.show();
    delay(50);
    for (int i = 0; i < stripBottom.numPixels(); i++) {
      stripBottom.setPixelColor(i, 25, 0, 0);
      stripTop.setPixelColor(i, 25, 0, 0);
    }
    stripBottom.show();
    stripTop.show();
    delay(200);
  }
}
*/
void RainbowPincers() {
  for (int j = 0; j < 10; j++) {
    for (int i = 0; i <= stripBottom.numPixels() / 2; i++) {
      stripBottom.setPixelColor(i, Wheel(map(i, 0, stripBottom.numPixels(), 0, 255)));
      stripBottom.setPixelColor(stripBottom.numPixels() - i, Wheel(map(i, 0, stripBottom.numPixels(), 255, 0)));
      stripTop.setPixelColor((int)(i / 2), Wheel(map((int)(i / 2), 0, (stripTop.numPixels() - BackPixelNum), 0, 255)));
      stripTop.setPixelColor((stripTop.numPixels() - BackPixelNum) - (int)(i / 2), Wheel(map((int)(i / 2), 0, (stripTop.numPixels() - BackPixelNum), 255, 0)));
      stripBottom.show();
    }
    for (int i = stripBottom.numPixels() / 2; i <= stripBottom.numPixels(); i++) {
      stripBottom.setPixelColor(i, 0, 0, 0);
      stripBottom.setPixelColor((stripBottom.numPixels() / 2) - (i - (stripBottom.numPixels() / 2)), 0, 0, 0);
      stripBottom.show();
    }    
  }
}

void TeamNumber() {
  for (int i = 10; i < 12; i++) {
    stripTop.setPixelColor(i, 255, 0, 0);
  }
  for (int i = 18; i < 26; i++) {
    stripTop.setPixelColor(i, 255, 0, 0);
  }
  for (int i = 32; i < 36; i++) {
    stripTop.setPixelColor(i, 255, 0, 0);
  }
  for (int i = 42; i < 48; i++) {
    stripTop.setPixelColor(i, 255, 0, 0);
  }
  stripTop.show();
  delay(10000);
}

