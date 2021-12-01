int shifterToggle1ButtonPin = 7;

void setup() {
  pinMode(shifterToggle1ButtonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(shifterToggle1ButtonPin) == HIGH) {
    Serial.println("PUSH!");
  } else {
    Serial.println("RELEASE!");
  }

  delay(500);
}
