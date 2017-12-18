#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            7

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      8

//Define notes.
#define DO 1
#define RE 2
#define MI 3
#define FA 4
#define SOL 5
#define LA 6
#define SI 7
#define OD 8
#define FIM 0

#define CC 1
#define DD 2
#define EE 3
#define FF 4
#define GG 5
#define AA 6
#define BB 7
#define CCC 8
#define FIMZ 0

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 100; // delay for half a second

// these constants won't change:
const int knockSensor[] =   {A0, A1, A2, A3, A4, A5, A6, A7};     // knock sensor connected from pin AX to AY
const int threshold[] = {200,150,300,100,200,200,200,200};                                        // threshold value to decide when the detected sound is a knock or not



// Each notes array represents a song.

//TEST
//int notes[] = {DO,FA,OD,RE,MI,LA,LA,RE,MI,SOL,SOL,LA,MI,FA,RE,DO,OD,OD,DO,DO,FA,OD,RE,MI,LA,LA,RE,MI,SOL,SOL,LA,MI,FA,RE,DO,OD,OD,DO,FIM};

//BRILA-BRILHA ESTRELINA
//int notes[] = {DO,DO,SOL,SOL,LA,LA,SOL,FA,FA,MI,MI,RE,RE,DO,SOL,SOL,FA,FA,MI,MI,RE,SOL,SOL,FA,FA,MI,MI,RE,DO,DO,SOL,SOL,LA,LA,SOL,FA,FA,MI,MI,RE,RE,DO,FIM};

//MUSICA NATALINA
int notes[] = {EE, EE, EE, EE, EE, EE, EE, GG, CC, DD, EE, FF, FF, FF, FF, FF, EE, EE, EE, GG, GG, FF, DD, DO, DO,FIM};


int sensorReading[] =      {0, 0, 0, 0, 0, 0, 0, 0};
int sensorReadingaux[] =   {0, 0, 0, 0, 0, 0, 0, 0};
int difs[] =               {0, 0, 0, 0, 0, 0, 0, 0};
   
int currentNoteIndex = 0;
int mustKnock = 0;


void setup() {
  
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
   if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
  
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval);
  }
  Serial.begin(9600);       // use the serial port

  // loadMusic(1);
  
  mustKnock = hashNote(notes[currentNoteIndex]);
  turnOn(mustKnock);
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, c);
    pixels.show();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, c);    //turn every third pixel on
      }
      pixels.show();

      delay(wait);

      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      pixels.show();

      delay(wait);

      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}


void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

void arcoIro(){
  Serial.print("pao");
  // Some example procedures showing how to display to the pixels:
  colorWipe(pixels.Color(255, 0, 0), 50); // Red
  colorWipe(pixels.Color(0, 255, 0), 50); // Green
  colorWipe(pixels.Color(0, 0, 255), 50); // Blue
//colorWipe(pixels.Color(0, 0, 0, 255), 50); // White RGBW
  // Send a theater pixel chase in...
  theaterChase(pixels.Color(127, 127, 127), 50); // White
  theaterChase(pixels.Color(127, 0, 0), 50); // Red
  theaterChase(pixels.Color(0, 0, 127), 50); // Blue

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
}

void loop() {
  
  int aux = 0;
  while(notes[mustKnock] > 0){
    sensorReadingaux[mustKnock] = sensorReading[mustKnock];
    sensorReading[mustKnock] = analogRead(knockSensor[mustKnock]);
    difs[mustKnock] = sensorReading[mustKnock] - sensorReadingaux[mustKnock];
    Serial.println(mustKnock);
    if(difs[mustKnock] > threshold[mustKnock]) {
      knockN(mustKnock);
      delay(100);
    }
  }
  Serial.println("saiu");
  arcoIro();
  delay(25);  // delay to avoid overloading the serial port buffer

}

int hashNote(int note) {
  return note - 1;
}

void updateWhoMustKnock() {
   currentNoteIndex++;
   
   mustKnock = hashNote(notes[currentNoteIndex]);
   Serial.print(mustKnock);
   turnOn(mustKnock);
}

void knockN(int i) {
  Serial.print("Knock ");
  Serial.print(i);
  Serial.println("!");

  //if (ledState[i] == HIGH) {
  if(true){
    turnOff(i);
    updateWhoMustKnock();
  } else {
    turnOn(i);
  }
  delay(50);
}

// Liga as luzes com as respectivas cores.
void turnOn(int i) {
  
  if(i==0){
    pixels.setPixelColor(i, pixels.Color(0,0,255)); // Moderately bright green color  
  }
  else if(i==1){
    pixels.setPixelColor(i, pixels.Color(255,0,0)); // Moderately bright green color  
  }
  else if(i==2){
    pixels.setPixelColor(i, pixels.Color(0,0,255)); // Moderately bright green color  
  }
  else if(i == 3 || i ==4 || i==6){
      pixels.setPixelColor(i, pixels.Color(0,255,0)); // Moderately bright green color
  }
  else if(i==5){
    pixels.setPixelColor(i, pixels.Color(0,0,255)); // Moderately bright green color  
  }
  else {
    pixels.setPixelColor(i, pixels.Color(255,0,0)); // Moderately bright green color
  }
  pixels.show();
  delay(delayval);
}

//Desliga a luz
void turnOff(int i) {

  pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color
  pixels.show();
  delay(delayval);
}

