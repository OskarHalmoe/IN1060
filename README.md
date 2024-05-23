# Erstatterne "Noe å ha i bokhylla" IN1060 2024

Denne koden er fra våren 2024 og består av c++ kode som er skrevet i Arduino IDE. Linken til vår prosjektside finner du her: [Erstatterne](https://www.uio.no/studier/emner/matnat/ifi/IN1060/v24/prosjektgrupper/erstatterne/)
. Der vil du finne litt om hvordan prosjektet vårt utartet seg gjennom vårsemesteret. Du vil også finne selve prototypen vår og hvordan koden som forklares her inne fungerer i praksis.

Hvis du bare er ute etter kodefilen så finner du den som en .ino fil overnfor.



Ну, всё, счастливого пути. Удачи!  😁

(ikke snakk til meg på russisk)

# Forklaring av kode


Her inkluderer vi biblioteket og definerer nødvendige deler av biblioteket for å kunne initiere LED-stripen.
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
  BUTTON_IDLE, 
  BUTTON_PRESSED, 
  BUTTON_RELEASED
};

ButtonState knappStatus = BUTTON_IDLE;
unsigned long sistKnappeTrykk = 0;
CRGBPalette16 palette1;
CRGBPalette16 palette2;
CRGBPalette16 palette3;
CRGBPalette16* currentPalette;
TBlendType    currentBlending;
```


## setup();

- Starter med en delay av sikkerhetsmessige grunner for både LEDene og systemet.
- Vi bruker feltene ovenfor for å initiere LED lysene.
- Bruker predefinerte paletter fra biblioteket for hvert av palettobjektene våre.
- Velger hvilket palett vi starter med, og velger hvilken blend(sjekk dokumentasjonen til biblioteket)
- Bestemmer hvordan vi ønsker at knappene våre skal oppføre seg.


```c++
void setup() {

    delay( 3000 );
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );

    palette1 = RainbowColors_p;
    palette2 = OceanColors_p;
    palette3 = LavaColors_p;
    currentPalette = &palette1;
    currentBlending = LINEARBLEND;

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(SWITCH_PIN, INPUT_PULLUP);
}
```

## loop();

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
## fyllLEDsMedPalettFarger(unit8_t fargeIndeks);

Formålet til denne metoden er å gi en estetisk fargeeffekt som bruker palettene vi har opprettet. 

- Vi setter lysstyrken til 255 (unit8_t er et 8-bits heltall objekt)
- Vi itererer gjenn hver eneste LED i lyslenka.
- Vi setter fargen til den nåværende LEDen ved å hente en farge fra det nåværende palettet.
- Går 3 farger frem i paletten for neste LED


```c++
void fyllLEDsMedPalettFarger(uint8_t fargeIndeks) {
    uint8_t lysstyrke = 255;
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(*currentPalette, fargeIndeks, lysstyrke, currentBlending);
        fargeIndeks += 3;
    }
}
```

## endrePalett();

- Sjekker hvilket av palett-objektene som er aktive
- Endrer til plaett 2 om palett 1 er aktiv
- Endrer til palett 3 om palett 2 er aktiv
- Endrer til palett 1 om palett 3 er aktiv


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
## haandterKnappeTrykk();

- Setter statusen til BUTTON_PIN i variablen knappStatusNaa.
- Sjekker statusen til knappen, og bruker enumerasjonsverdiene vi laget øverst for å bedømme hvilken tilstand knappen har.
- Når det har gått tilstrekkelig med tid (se koden for hvor mye tid jeg har bestemt), kaller vi på endrePalett metoden.
- Så setter vi knappen tilbake til "ikke-aktiv" status "BUTTON_IDLE".

```c++
void haandterKnappeTrykk(){
  int knappStatusNaa = digitalRead(BUTTON_PIN);
  if (knappStatusNaa == LOW && knappStatus != BUTTON_PRESSED){
    knappStatus = BUTTON_PRESSED;
    sistKnappeTrykk = millis();
  } else if (knappStatusNaa == HIGH && knappStatus == BUTTON_PRESSED){
    knappStatus = BUTTON_RELEASED;
  } 
  if (knappStatus == BUTTON_PRESSED && millis() - sistKnappeTrykk >= 100){
    Serial.println("knapp -> OFF -> ON");
    endrePalett();
    knappStatus = BUTTON_IDLE;
  }
}
```
## haandterLEDs();

- Oppretter et uint8_t objekt.
- Bruker 8-bits objektet som parameter i metoden for å fylle opp lyslenken.
- Sørger for at LEDene oppdateres og regulerer hvor ofte de kan oppdateres per sekund. 
```c++
void haandterLEDs(){
  static uint8_t startIndeks = 0;
  startIndeks++;
  fyllLEDsMedPalettFarger(startIndeks);
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}
```
## skruAvLEDs();

- Skrur av LEDs ;)

```c++
void skruAvLEDs(){
  FastLED.clear();
  FastLED.show();
}

```
