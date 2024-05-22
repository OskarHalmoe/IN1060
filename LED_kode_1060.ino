
#include <FastLED.h>

#define LED_PIN         6 //forandre basert på pin
#define NUM_LEDS        60 //forandre basert på ant LED i lenken
#define BRIGHTNESS      64
#define LED_TYPE        WS2812B //forandre basert på type led
#define COLOR_ORDER     GRB //forandre basert på hvordan du vil ha fargene organisert
#define BUTTON_PIN      8 //forandre basert på pin
#define SWITCH_PIN      12 //forandre basert på pin
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100

//oppretter tilstander for knappene
enum ButtonState {
  BUTTON_IDLE, //knapp ikke trykket
  BUTTON_PRESSED, //knapp trykket
  BUTTON_RELEASED //knapp sluppet
};

//Setter knappen til ikke trykket.
ButtonState knappStatus = BUTTON_IDLE;
//Variabel for å lagre når knapp ble trykket (sjekk metode for håndtering av knapp) 
unsigned long sistKnappeTrykk = 0;
//Opretter palett-objekter.
CRGBPalette16 palette1;
CRGBPalette16 palette2;
CRGBPalette16 palette3;
//Lager variablen nåværende palett.
CRGBPalette16* currentPalette;
//bestemmer Blendtype til palettet. 
TBlendType    currentBlending;


void setup() {
    delay( 3000 ); // power-up sikkerhet for LED lyset. 
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    //opretter palettene
    palette1 = RainbowColors_p;
    palette2 = OceanColors_p;
    palette3 = LavaColors_p;
    // Starter med palett1
    currentPalette = &palette1;
    // Type blend i fargene. 
    currentBlending = LINEARBLEND;
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(SWITCH_PIN, INPUT_PULLUP);

}

void loop() {
  //Setter status på switchen vår i en variabel.
  int switchStatus = digitalRead(SWITCH_PIN);
  //Sjekker om switchen er PÅ/HIGH.
  if (switchStatus == HIGH) {
    //Håndterer bruk av knapp som endrer palett, og starter lysene. 
    haandterKnappeTrykk();
    haanderLEDs();
  } else {
    //Skrur av lysene.
    skruAvLEDs();
  }
}

//Metode for å fylle LEDene med lys.
void fyllLEDsMedPalettFarger(uint8_t fargeIndeks) {
    uint8_t lysstyrke = 255;
    //For hver LED vi fyller, hopper vi 3 farger inn i palettet slik at alle LEDene blir nogenlunde forskjellige.
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(*currentPalette, fargeIndeks, lysstyrke, currentBlending);
        fargeIndeks += 3;
    }
}

// Skift mellom palett 1,2 og 3. 
void endrePalett() {
    if (currentPalette == &palette1) {
        currentPalette = &palette2;
    } else if(currentPalette == &palette2){
        currentPalette = &palette3;
    } else{
        currentPalette = &palette1;
    }
}

//Metode for å håndtere bruk av knapp.
void haandterKnappeTrykk(){
  //Leser knappen
  int knappStatusNaa = digitalRead(BUTTON_PIN);
  //sjekker om knappen er trykket
  if (knappStatusNaa == LOW && knappStatus != BUTTON_PRESSED){
    knappStatus = BUTTON_PRESSED;
    sistKnappeTrykk = millis();
  }else if (knappStatusNaa == HIGH && knappStatus == BUTTON_PRESSED){
    knappStatus = BUTTON_RELEASED;
  }
  //forsikrer oss om at det har gått lang nok tid fra knappetrykket til palettet endrer seg. 
  if (knappStatus == BUTTON_PRESSED && millis() - sistKnappeTrykk >= 100){
    Serial.println("knapp -> OFF -> ON");
    //endrer palett
    endrePalett();
    knappStatus = BUTTON_IDLE;
  }
}

//Her bruker vi metoden som fyller LEDene og oppdaterer den slik at lysene "flyter".
void haandterLEDs(){
  static uint8_t startIndeks = 0;
  startIndeks++;
  fyllLEDsMedPalettFarger(startIndeks);
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

//Skrur av LEDene. 
void skruAvLEDs(){
  FastLED.clear();
  FastLED.show();
}
