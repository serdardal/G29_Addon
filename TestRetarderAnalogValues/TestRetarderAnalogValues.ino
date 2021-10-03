int retarderSensorValue;

void setup() {
  Serial.begin(9600);
}

void loop() {
  retarderSensorValue = analogRead(A0);
  Serial.print("Current potantiometer value: ");
  Serial.println(retarderSensorValue);

  delay(500);
}
