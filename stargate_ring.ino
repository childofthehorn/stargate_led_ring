#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

//Attach Intterupt
//Pin 7 on Pro Micro 
//Pin 2/3 on ATMEGA 328/168 (using 2) 
#define INTERRUPT 2

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed: 
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(144, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

boolean firstRun = true;

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(INTERRUPT, INPUT_PULLUP);
  //Have it do the White Theater Chase when "flashing"
  attachInterrupt(digitalPinToInterrupt(INTERRUPT),lightSequence, FALLING);
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  //Lets make this look like the "Real Deal"
  //colorWipeChase(strip.Color(255, 100, 10), 400); // Yellow
  if(firstRun){
    firstRun = false;
    fireSequence(strip.Color(255, 100, 10), 300);
    delay(1000);
    colorWipeBlue(0);
    delay(100);
  }
  theaterChaseRainbow(0);
  theaterChaseRainbow(0);
  theaterChaseRainbow(0);
}

void lightSequence(){
  theaterChase(strip.Color(127, 127, 127), 20);
  theaterChase(strip.Color(127, 127, 127), 20);
  theaterChase(strip.Color(127, 127, 127), 20);
  theaterChase(strip.Color(127, 127, 127), 20);
  theaterChase(strip.Color(127, 127, 127), 20);
  theaterChase(strip.Color(127, 127, 127), 20);
  fireSequence(strip.Color(255, 100, 10), 300);
  delay(1000);
  colorWipeBlue(0);
  delay(100);
  loop();
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// Fill the dots one after the other with a color
void colorWipeBlue( uint8_t wait) {
  for(uint16_t f=60; f>0; f--){
   uint32_t c = strip.Color(f,f, 127);
   f--;
   f--;
   f--;
   f--;
   f--;
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
  }
}

// Fill the dots one after the other with a color (every other)
void colorWipeChase(uint32_t c, uint16_t wait) {
  for(uint16_t i=0; i<strip.numPixels()+3; i++) {
    strip.setPixelColor(i, c);
    i++;
    strip.setPixelColor(i, c);
    i++;
    strip.setPixelColor(i, c);
    i++;
    strip.setPixelColor(i, strip.Color(0,0,0));
    i++;
    strip.setPixelColor(i, strip.Color(0,0,0));
    i++;
    strip.setPixelColor(i, strip.Color(0,0,0));
    i++;
    strip.setPixelColor(i, strip.Color(0,0,0));
    i++;
    strip.setPixelColor(i, strip.Color(0,0,0));
    i++;
    strip.setPixelColor(i, strip.Color(0,0,0));
    i++;
    strip.setPixelColor(i, strip.Color(0,0,0));
    i++;
    strip.setPixelColor(i, strip.Color(0,0,0));
    i++;
    strip.setPixelColor(i, strip.Color(0,0,0));
    i++;
    strip.setPixelColor(i, strip.Color(0,0,0));
    i++;
    strip.setPixelColor(i, strip.Color(0,0,0));
    i++;
    strip.setPixelColor(i, strip.Color(0,0,0));
    i++;
    strip.setPixelColor(i, c);
    i++;
    strip.setPixelColor(i, c);
    i++;
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  //Disable the interrupts while going through the theater chase
  noInterrupts();
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
  cli ();
  interrupts();
  //Renable once done.
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+2) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+2) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


void fireSequence(uint32_t c, uint16_t wait ){
  five(c);
  delay(wait/2);
  six(c);
  delay(wait/5);
  fiveOff();
  delay(wait);
  
  five(c);
  delay(wait/2);
  seven(c);
  delay(wait/5);
  fiveOff();
  delay(wait);

  five(c);
  delay(wait/2);
  eight(c);
  delay(wait/5);
  fiveOff();
  delay(wait);

  five(c);
  delay(wait/2);
  one(c);
  delay(wait/5);
  fiveOff();
  delay(wait);

  five(c);
  delay(wait/2);
  two(c);
  delay(wait/5);
  fiveOff();
  delay(wait);

  five(c);
  delay(wait/2);
  three(c);
  delay(wait/5);
  fiveOff();
  delay(wait);

  five(c);
  delay(wait/2);
  four(c);
  delay(wait/5);
  fiveOff();
  delay(wait);

  five(c);
  delay(wait);
}

void one(uint32_t c){
  strip.setPixelColor(8, strip.Color(0,0,0));
  strip.setPixelColor(7, strip.Color(0,0,0));
  strip.setPixelColor(6, strip.Color(0,0,0));
  strip.setPixelColor(5, strip.Color(0,0,0));
  strip.setPixelColor(4, strip.Color(0,0,0));
  strip.setPixelColor(3, strip.Color(0,0,0));
  strip.setPixelColor(0, c);
  strip.setPixelColor(1, c);
  strip.setPixelColor(2, c);
  strip.setPixelColor(143, c);
  strip.setPixelColor(142, c);
  strip.setPixelColor(141, c);
  strip.setPixelColor(140, strip.Color(0,0,0));
  strip.setPixelColor(139, strip.Color(0,0,0));
  strip.setPixelColor(138, strip.Color(0,0,0));
  strip.setPixelColor(137, strip.Color(0,0,0));
  strip.setPixelColor(136, strip.Color(0,0,0));
  strip.setPixelColor(135, strip.Color(0,0,0));
  strip.show();
}

void two(uint32_t c){
  int i = 134;
  segmentLight(i,c);
}
void three(uint32_t c){
  int i = 116;
  segmentLight(i,c);
}
void four(uint32_t c){
  int i = 98;
  segmentLight(i,c);
}
void five(uint32_t c){
  int i = 80;
  segmentLight(i,c);
}
void six(uint32_t c){
  int i = 62;
  segmentLight(i,c);
}
void seven(uint32_t c){
  int i = 44;
  segmentLight(i,c);
}
void eight(uint32_t c){
  int i = 26;
  segmentLight(i,c);
}

void fiveOff(){
  int i = 80;
  segmentOff(i);
}



void segmentLight(int i, uint32_t c){
  strip.setPixelColor(i, strip.Color(0,0,0));
  strip.setPixelColor(i-1, strip.Color(0,0,0));
  strip.setPixelColor(i-2, strip.Color(0,0,0));
  strip.setPixelColor(i-3, strip.Color(0,0,0));
  strip.setPixelColor(i-4, strip.Color(0,0,0));
  strip.setPixelColor(i-5, strip.Color(0,0,0));
  strip.setPixelColor(i-6, c);
  strip.setPixelColor(i-7, c);
  strip.setPixelColor(i-8, c);
  strip.setPixelColor(i-9, c);
  strip.setPixelColor(i-10, c);
  strip.setPixelColor(i-11, c);
  strip.setPixelColor(i-12, strip.Color(0,0,0));
  strip.setPixelColor(i-13, strip.Color(0,0,0));
  strip.setPixelColor(i-14, strip.Color(0,0,0));
  strip.setPixelColor(i-15, strip.Color(0,0,0));
  strip.setPixelColor(i-16, strip.Color(0,0,0));
  strip.setPixelColor(i-17, strip.Color(0,0,0));
  strip.show();
}

void segmentOff(int i){
  strip.setPixelColor(i, strip.Color(0,0,0));
  strip.setPixelColor(i-1, strip.Color(0,0,0));
  strip.setPixelColor(i-2, strip.Color(0,0,0));
  strip.setPixelColor(i-3, strip.Color(0,0,0));
  strip.setPixelColor(i-4, strip.Color(0,0,0));
  strip.setPixelColor(i-5, strip.Color(0,0,0));
  strip.setPixelColor(i-6, strip.Color(0,0,0));
  strip.setPixelColor(i-7, strip.Color(0,0,0));
  strip.setPixelColor(i-8, strip.Color(0,0,0));
  strip.setPixelColor(i-9, strip.Color(0,0,0));
  strip.setPixelColor(i-10, strip.Color(0,0,0));
  strip.setPixelColor(i-11, strip.Color(0,0,0));
  strip.setPixelColor(i-12, strip.Color(0,0,0));
  strip.setPixelColor(i-13, strip.Color(0,0,0));
  strip.setPixelColor(i-14, strip.Color(0,0,0));
  strip.setPixelColor(i-15, strip.Color(0,0,0));
  strip.setPixelColor(i-16, strip.Color(0,0,0));
  strip.setPixelColor(i-17, strip.Color(0,0,0));
  strip.show();
}



