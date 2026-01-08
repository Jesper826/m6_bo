// ---------------------------------------------------
// PINS
// ---------------------------------------------------
const int potPinVolume = A1;
const int potPinEffect = A2;

int volumeLeds[] = {3, 4, 5, 6};
const int totaalVolumeLEDs = 4;

int ledsTokkel[] = {8, 9, 10, 11};
const int totaalTokkelLEDs = 4;

const int speakerPinTok  = 7;
const int speakerPinLofi = 12;

const int ldrPin    = A3;
const int buttonPin = 2;

// ---------------------------------------------------
// BUTTON / SYSTEM
// ---------------------------------------------------
bool systemOn = false;
bool lastButtonState = HIGH;

// ---------------------------------------------------

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < totaalTokkelLEDs; i++) pinMode(ledsTokkel[i], OUTPUT);
  for (int i = 0; i < totaalVolumeLEDs; i++) pinMode(volumeLeds[i], OUTPUT);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(speakerPinTok, OUTPUT);
  pinMode(speakerPinLofi, OUTPUT);
}

// ---------------------------------------------------

void loop() {
  // ---------------- BUTTON TOGGLE ----------------
  int reading = digitalRead(buttonPin);

  if (lastButtonState == HIGH && reading == LOW) {
    systemOn = !systemOn;
    Serial.print("SystemOn: ");
    Serial.println(systemOn);
    delay(150); // debounce
  }
  lastButtonState = reading;

  // ---------------- SYSTEM UIT ----------------
  if (!systemOn) {
    noTone(speakerPinTok);
    noTone(speakerPinLofi);

    for (int i = 0; i < totaalVolumeLEDs; i++) digitalWrite(volumeLeds[i], HIGH);
    for (int i = 0; i < totaalTokkelLEDs; i++) digitalWrite(ledsTokkel[i], HIGH);
    return;
  }

  // ------------------------------------------------
  // VOLUME POT
  // ------------------------------------------------
  int rawVolume = analogRead(potPinVolume);
  int volume = map(rawVolume, 0, 1023, 0, totaalVolumeLEDs);

  for (int i = 0; i < totaalVolumeLEDs; i++) {
    digitalWrite(volumeLeds[i], i < volume ? HIGH : LOW);
  }

  int volumeDuur = 40 + volume * 60;

  // ------------------------------------------------
  // EFFECT POT
  // ------------------------------------------------
  int rawEffect = analogRead(potPinEffect);

  int pitchOffset  = map(rawEffect, 0, 1023, -80, 180);
  int vibratoDepth = map(rawEffect, 0, 1023, 2, 18);
  int vibratoRate  = map(rawEffect, 0, 1023, 2, 8);

  // ------------------------------------------------
  // TOKKEL
  // ------------------------------------------------
  int waarde = analogRead(A0);
  int tokkelNiveau = map(waarde, 0, 1023, 0, totaalTokkelLEDs);

  for (int i = 0; i < totaalTokkelLEDs; i++) {
    digitalWrite(ledsTokkel[i], i < tokkelNiveau ? HIGH : LOW);
  }

  if (tokkelNiveau > 0) {
    tone(speakerPinTok, 260 + pitchOffset, volumeDuur);
    delay(volumeDuur + 30);
  } else {
    noTone(speakerPinTok);
  }

  // ------------------------------------------------
  // LOFI + LDR (ZWARE BEATS)
  // ------------------------------------------------
  int ldrValue = analogRead(ldrPin);

  if (ldrValue < 20) {
    int tempoDuur = 170;

    heavyBeat(110, volumeDuur, vibratoDepth, vibratoRate);
    delay(tempoDuur);

    heavyBeat(140, volumeDuur, vibratoDepth, vibratoRate);
    delay(tempoDuur);
  }
  else if (ldrValue > 35) {
    int tempoDuur = 95;

    heavyBeat(190, volumeDuur, vibratoDepth, vibratoRate);
    delay(tempoDuur);
  }

  delay(20);
}

// ---------------------------------------------------
// HEAVY BEAT (ZWARE KLAP)
// ---------------------------------------------------
void heavyBeat(int freq, int duration, int depthHz, int rateHz) {
  // korte lage kick
  tone(speakerPinLofi, freq - 35, 30);
  delay(35);

  // hoofdtoon met vibrato
  playLofiWithVibrato(freq, duration, depthHz, rateHz);
}

// ---------------------------------------------------
// LOFI MET VIBRATO
// ---------------------------------------------------
void playLofiWithVibrato(int freq, int duration, int depthHz, int rateHz) {

  if (depthHz <= 0 || rateHz <= 0) {
    tone(speakerPinLofi, freq, duration);
    return;
  }

  unsigned long start = millis();

  while (millis() - start < (unsigned long)duration) {
    float t = (millis() - start) / 1000.0;
    float offset = depthHz * sin(2.0 * 3.14159 * rateHz * t);
    tone(speakerPinLofi, freq + offset, 12);
    delay(12);
  }
}
