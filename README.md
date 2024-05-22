# Erstatterne "Noe å ha i bokhylla" IN1060 2024

- Mye rart

- hve er vi?
- hva er dette?
- hvorfor er dette her?
- Dette er for oss


# Forklaring av kode


Her inkluderer jeg biblioteket og definenerer nødvendige deler av biblioteket for å kunne initiere LED-stripen.
```c++

#include <FastLED.h>
#define LED_PIN         6
#define NUM_LEDS        60
#define BRIGHTNESS      64
#define LED_TYPE        WS2812B
#define COLOR_ORDER     GRB
#define BUTTON_PIN      8 
#define SWITCH_PIN      12 
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100
```


Nedenfor ser du en enumerasjon av knapp_statuser og hvordan man initierer diverse paletter som biblioteket FastLED tilbyr.

```c++

enum ButtonState {

  BUTTON_IDLE, //knapp ikke trykket
  BUTTON_PRESSED, //knapp trykket
  BUTTON_RELEASED //knapp sluppet

};

ButtonState knappStatus = BUTTON_IDLE;
unsigned long sistKnappeTrykk = 0;

CRGBPalette16 palette1;
CRGBPalette16 palette2;
CRGBPalette16 palette3;
 
CRGBPalette16* currentPalette;
TBlendType    currentBlending;
```


I setup():

- vi feltene ovenfor for å initiere LED lysene.
- bruker predefinerte paletter fra biblioteket for hvert av palettobjektene våre.
- Velger hvilket palett vi starter med, og velger hvilken blend(sjekk dokumentasjonen til biblioteket)
- Bestemmer hvordan vi ønsker at knappene våre skal oppføre seg.


```c++
void setup() {

    delay( 3000 ); // power-up sikkerhet for LED lyset. 
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );

    //opretter palettene

    palette1 = RainbowColors_p;
    palette2 = OceanColors_p;
    palette3 = LavaColors_p;
    currentPalette = &palette1; // Starter med palett1
    currentBlending = LINEARBLEND;
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(SWITCH_PIN, INPUT_PULLUP);
}
```

I loop():

- Leser statusen til switchpin og setter den i variablen switchStatus.
- Sjekker status på bryteren, om HIGH kaller vi på metodene for knapp & lyshåndtering.
- Skrur av lysene om LOW.

```c++
void loop() {

  int switchStatus = digitalRead(SWITCH_PIN);
  if (switchStatus == HIGH) {
    haandterKnappeTrykk();
    haanderLEDs();

  } else {
    skruAvLEDs();
  }
}
```
okoko

```c++
void fyllLEDsMedPalettFarger(uint8_t fargeIndeks) {
    uint8_t lysstyrke = 255;

    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(*currentPalette, fargeIndeks, lysstyrke, currentBlending);
        fargeIndeks += 3;
    }
}
```

```c++
void endrePalett() {
    if (currentPalette == &palette1) {
        currentPalette = &palette2;
    } else if(currentPalette == &palette2){
        currentPalette = &palette3;
    } else {
        currentPalette = &palette1;
    }
}
```

```c++
void haandterKnappeTrykk(){

  //Leser knappen
  int knappStatusNaa = digitalRead(BUTTON_PIN);

  //sjekker om knappen er trykket
  if (knappStatusNaa == LOW && knappStatus != BUTTON_PRESSED){
    knappStatus = BUTTON_PRESSED;
    sistKnappeTrykk = millis();
  } else if (knappStatusNaa == HIGH && knappStatus == BUTTON_PRESSED){
    knappStatus = BUTTON_RELEASED;
  }
  //forsikrer oss om at det har gått lang nok tid fra knappetrykket til palettet endrer seg. 
  if (knappStatus == BUTTON_PRESSED && millis() - sistKnappeTrykk >= 100){
    Serial.println("knapp -> OFF -> ON");
    endrePalett();
    knappStatus = BUTTON_IDLE;
  }
}
```

```c++
void haandterLEDs(){

  static uint8_t startIndeks = 0;
  startIndeks++;

  fyllLEDsMedPalettFarger(startIndeks);

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);

}
```

```c++
void skruAvLEDs(){

  FastLED.clear();
  FastLED.show();

}

```
