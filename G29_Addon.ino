#include <Joystick.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   5, 0,                  // Button Count, Hat Switch Count
                   false, false, false,   // No X, Y, or Z Axis
                   false, false, false,   // No Rx, Ry, or Rz
                   false, false,          // No rudder or throttle
                   false, false, false);  // No accelerator, brake, or steering

int retarderSensorValue;
int retarderPositionValues[] = {579, 520, 455, 398};

int retarderCurrentPos = 0;
int retarderPotValueMargin = 7;

bool shifterToggle1ReadValue;
int shifterToggle1ButtonPin = 4;
bool shifterToggle1State = 0;

bool shifterToggle2ReadValue;
int shifterToggle2ButtonPin = 7;
bool shifterToggle2State = 0;

bool handBrakeReadValue;
int handBrakeButtonPin = 2;
bool handBrakeState = 0;

void setup() {
  pinMode(shifterToggle1ButtonPin, INPUT);
  pinMode(shifterToggle2ButtonPin, INPUT);
  pinMode(handBrakeButtonPin, INPUT);
  Joystick.begin();
}

void loop() {
  retarderSensorValue = analogRead(A0);
  shifterToggle1ReadValue = digitalRead(shifterToggle1ButtonPin) == HIGH;
  shifterToggle2ReadValue = digitalRead(shifterToggle2ButtonPin) == HIGH;
  handBrakeReadValue = digitalRead(handBrakeButtonPin) == HIGH;

  //retarder kullanılmayacaksa aşağıdaki satır yorum yapılmalı
  //handleRetarder();
  handleShifterToggle();
  handleHandBrake();

  delay(1);
}

void handleRetarder() {
  int newRetarderPos = getNewRetarderPos(retarderSensorValue);

  if (newRetarderPos >= 0 && newRetarderPos != retarderCurrentPos) {
    adjustRetarderPosition(newRetarderPos);
  }
}

int getNewRetarderPos(int value) {
  for (int i = 0; i < 4 ; i++) {
    if (abs(value - retarderPositionValues[i]) < retarderPotValueMargin) {
      return i;
    }
  }
  return -1;
}

void adjustRetarderPosition(int newPos) {
  bool turnDirection = newPos > retarderCurrentPos;
  int diff = abs(newPos - retarderCurrentPos);
  for (int i = 0; i < diff; i++) {

    if (turnDirection) {
      ++retarderCurrentPos;
      pushToButton(0);
    } else {
      --retarderCurrentPos;
      pushToButton(1);
    }

    if (i >= 0 && i < (diff - 1)) {
      // delay between push button if diff > 1
      delay(50);
    }
  }
}

void handleShifterToggle() {
  if (shifterToggle1ReadValue != shifterToggle1State) {
    shifterToggle1State = shifterToggle1ReadValue;
    pushToButton(3);
  }

  if (shifterToggle2ReadValue != shifterToggle2State) {
    shifterToggle2State = shifterToggle2ReadValue;
    pushToButton(4);
  }
}

void handleHandBrake() {
  if (handBrakeReadValue != handBrakeState) {
    handBrakeState = handBrakeReadValue;
    if (handBrakeReadValue) {
      Joystick.pressButton(2);
    } else {
      Joystick.releaseButton(2);
    }
  }
}

void pushToButton(int buttonNum) {
  Joystick.pressButton(buttonNum);
  delay(10);
  Joystick.releaseButton(buttonNum);
}
