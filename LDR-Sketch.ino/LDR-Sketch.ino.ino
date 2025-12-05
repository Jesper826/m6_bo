const int ldrPin = A0;
const int speakerPin01 = A1;
const int speakerPin02 = A2;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int ldrValue = analogRead(ldrPin);

  Serial.print("LDR: ");
  Serial.println(ldrValue);

  // ---------------------------
  // LOFI BEAT 1 – very calm
  // ---------------------------
  if (ldrValue < 20) {
    tone(speakerPin01, 180, 250); // warm low tone
    delay(300);
    tone(speakerPin01, 220, 200); // gentle follow-up
    delay(500);                 // space = lofi
  }

  // ---------------------------
  // LOFI BEAT 2 – chill groove
  // ---------------------------
  else if (ldrValue > 20 && ldrValue < 30) {
    tone(speakerPin02, 250, 200);
    delay(250);
    tone(speakerPin02, 300, 200);
    delay(250);
    tone(speakerPin02, 220, 300); // bassy drop
    delay(400);
  }

  // ---------------------------
  // LOFI BEAT 3 – slightly upbeat but still soft
  // ---------------------------
  else if (ldrValue > 35) {
    tone(speakerPin01, 300, 180);
    delay(200);
    tone(speakerPin02, 350, 180);
    delay(200);
    tone(speakerPin01, 280, 250);
    delay(350);
  }

  delay(50);
}
