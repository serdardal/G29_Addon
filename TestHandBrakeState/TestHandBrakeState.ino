int handBrakeButtonPin = 2;

void setup() {
  pinMode(handBrakeButtonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(handBrakeButtonPin) == HIGH) {
    Serial.println("PUSH!");
  } else {
    Serial.println("RELEASE!");
  }

  delay(500);
}
