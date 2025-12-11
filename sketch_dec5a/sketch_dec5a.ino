// --- Pin definities ---

// Potmeter voor geluidsniveau (4 LEDs)
const int potPin = A1;

// 4 LED-pins voor geluidsniveau (LOFI-level)
int levelLeds[] = {3, 4, 5, 6};
const int aantalLevelLEDs = 4;

// 4 gewone LEDs voor tokkel-functie
int leds[] = {8, 9, 10, 11};
const int aantalLEDs = 4;

// Tokkel-geluid (heeft 5 niveaus!)  
const int totaalTokNiveaus = 5;

const int speakerPinTok = 2;
const int speakerPinLofi = 12;

// LDR
const int ldrPin = A3;



void setup() {
  for (int i = 0; i < aantalLEDs; i++) {
    pinMode(leds[i], OUTPUT);
  }

  for (int i = 0; i < aantalLevelLEDs; i++) {
    pinMode(levelLeds[i], OUTPUT);
  }

  pinMode(speakerPinTok, OUTPUT);
  pinMode(speakerPinLofi, OUTPUT);
}



void loop() {

  // --------------------------------------------------------
  // POTMETER → 4 LED NIVEAUMETER (LOFI LEVEL)
  // --------------------------------------------------------
  int waardeLevel = analogRead(potPin);
  int level = map(waardeLevel, 0, 1023, 0, aantalLevelLEDs); // 0–4

  for (int i = 0; i < aantalLevelLEDs; i++) {
    digitalWrite(levelLeds[i], i < level ? HIGH : LOW);
  }



  // --------------------------------------------------------
  // A0 → 5 niveaus (maar slechts 4 LEDs)
  // --------------------------------------------------------
  int waarde = analogRead(A0);
  int niveauTokkel = map(waarde, 0, 1023, 0, totaalTokNiveaus); // 0–5
  if (niveauTokkel > totaalTokNiveaus) niveauTokkel = totaalTokNiveaus;

  // LEDs tonen slechts 0–4 (4 LEDs)
  for (int i = 0; i < aantalLEDs; i++) {
    digitalWrite(leds[i], i < niveauTokkel ? HIGH : LOW);
  }

  // Tokkel-geluid blijft 5 stappen gebruiken
  if (niveauTokkel > 0) {
    int freq = 500;
    int tokDuur = 20;

    // 5 niveaus → lagere pauze per niveau
    int pauze = 300 - niveauTokkel * 45;
    if (pauze < 60) pauze = 60;

    tone(speakerPinTok, freq, tokDuur);
    delay(pauze);
  } else {
    noTone(speakerPinTok);
  }



  // --------------------------------------------------------
  // LDR → LOFI BEATS
  // --------------------------------------------------------
  int ldrValue = analogRead(ldrPin);

  if (ldrValue > 20 && ldrValue < 35) {
    tone(speakerPinLofi, 180, 250);
    delay(300);
    tone(speakerPinLofi, 220, 200);
    delay(500);
  }
  else if (ldrValue > 35) {
    tone(speakerPinLofi, 300, 180);
    delay(200);
  }

  delay(50);
}
