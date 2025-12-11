// --- Pins ---
const int potPinVolume = A1;
const int potPinEffect = A2;

int volumeLeds[] = {3, 4, 5, 6};
const int totaalVolumeLEDs = 4;

int ledsTokkel[] = {8, 9, 10, 11};
const int totaalTokkelLEDs = 4;

const int speakerPinTok = 7;
const int speakerPinLofi = 12;

const int ldrPin = A3;
const int buttonPin = 2;

// --- Toggle Button Variables ---
bool systemOn;
bool lastButtonState;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < totaalTokkelLEDs; i++) pinMode(ledsTokkel[i], OUTPUT);
  for (int i = 0; i < totaalVolumeLEDs; i++) pinMode(volumeLeds[i], OUTPUT);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(speakerPinTok, OUTPUT);
  pinMode(speakerPinLofi, OUTPUT);
}

void loop() {
  // -------------------------------------------------------
  // BUTTON TOGGLE LOGIC (ON/OFF SWITCH)
  // -------------------------------------------------------
  int reading = digitalRead(buttonPin);

if ( reading != lastButtonState && reading == 0) {
   Serial.println("Verander");

   // verander de state van systemOn
   systemOn = (false==systemOn);
   Serial.println(systemOn);
}

lastButtonState = reading;



  //Serial.print("Reading: "); Serial.print(reading);
  //Serial.print(" | SystemOn: "); Serial.println(systemOn);

  if (!systemOn) {
    noTone(speakerPinTok);
    noTone(speakerPinLofi);
    

    for (int i = 0; i < totaalVolumeLEDs; i++) digitalWrite(volumeLeds[i], HIGH);
    for (int i = 0; i < totaalTokkelLEDs; i++) digitalWrite(ledsTokkel[i], HIGH);
    return;
  }

  // -------------------------------------------------------
  // SYSTEM IS ON → Continue with normal code
  // -------------------------------------------------------

  // -------------------------
  // Volume pot (A1)
  // -------------------------
  int rawVolume = analogRead(potPinVolume);
  int volume = map(rawVolume, 0, 1023, 0, totaalVolumeLEDs);
  for (int i = 0; i < totaalVolumeLEDs; i++) {
    digitalWrite(volumeLeds[i], i < volume ? HIGH : LOW);
  }
  int volumeDuur = 30 + volume * 50;

  // -------------------------
  // Effect pot (A2)
  // -------------------------
  int rawEffect = analogRead(potPinEffect);
  int effectLevel = map(rawEffect, 0, 1023, 0, totaalTokkelLEDs);

  int pitchOffset  = map(rawEffect, 0, 1023, -80, 180);
  int vibratoDepth = map(rawEffect, 0, 1023, 0, 15);
  int vibratoRate  = map(rawEffect, 0, 1023, 0, 8);

  // -------------------------
  // Tokkel – lagere toon
  // -------------------------
  int waarde = analogRead(A0);
  int tokkelNiveau = map(waarde, 0, 1023, 0, totaalTokkelLEDs);
  for (int i = 0; i < totaalTokkelLEDs; i++) {
    digitalWrite(ledsTokkel[i], i < tokkelNiveau ? HIGH : LOW);
  }

  if (tokkelNiveau > 0) {
    int baseFreq = 260 + pitchOffset;
    tone(speakerPinTok, baseFreq, volumeDuur);
    delay(volumeDuur + 30);
  } else {
    noTone(speakerPinTok);
  }

  // -------------------------
  // Lofi – lagere tonen
  // -------------------------
  int ldrValue = analogRead(ldrPin);

  if (ldrValue < 20) {
    int tempoDuur = 160;
    playLofiWithVibrato(120, volumeDuur, vibratoDepth, vibratoRate);
    delay(tempoDuur);
    playLofiWithVibrato(150, volumeDuur, vibratoDepth, vibratoRate);
    delay(tempoDuur);
  } else if (ldrValue > 35) {
    int tempoDuur = 90;
    playLofiWithVibrato(200, volumeDuur, vibratoDepth, vibratoRate);
    delay(tempoDuur);
  }


  delay(20);
}

void playLofiWithVibrato(int freq, int duration, int depthHz, int rateHz) {
  if (depthHz <= 0 || rateHz <= 0) {
    tone(speakerPinLofi, freq, duration);
    return;
  }

  unsigned long start = millis();
  unsigned long now = start;

  while (now - start < (unsigned long)duration) {
    float t = (now - start) / 1000.0;
    float offset = depthHz * sin(2.0 * 3.14159 * rateHz * t);
    int curFreq = freq + (int)offset;

    tone(speakerPinLofi, curFreq, 10);
    delay(10);
    now = millis();
  }
}
