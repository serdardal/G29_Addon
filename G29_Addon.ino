#include <Joystick.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   4, 0,                  // Button Count, Hat Switch Count
                   false, false, false,   // No X, Y, or Z Axis
                   false, false, false,   // No Rx, Ry, or Rz
                   false, false,          // No rudder or throttle
                   false, false, false);  // No accelerator, brake, or steering

int retarderSensorValue;
int retarderPositionValues[] = {229, 329, 422, 519};
// 0 means retarder is closed.
int mappedInGameRetarderPositions[] = {0, 2, 3, 5};

int retarderCurrentPos = 0;
int retarderInGamePos = 0;
int retarderPotValueMargin = 7;

bool shifterToggle1ReadValue;
int shifterToggle1ButtonPin = 2;
bool shifterToggle1State = 0;

bool handBrakeReadValue;
int handBrakeButtonPin = 4;
bool handBrakeState = 0;

void setup() {
  pinMode(shifterToggle1ButtonPin, INPUT);
  pinMode(handBrakeButtonPin, INPUT);
  Joystick.begin();
}

void loop() {
  retarderSensorValue = analogRead(A0);
  shifterToggle1ReadValue = digitalRead(shifterToggle1ButtonPin) == HIGH;
  handBrakeReadValue = digitalRead(handBrakeButtonPin) == HIGH;

  handleRetarder();
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
  if (turnDirection) {
    increaseRetarderPos(newPos);
  } else {
    decreaseRetarderPos(newPos);
  }
}

void increaseRetarderPos(int newPos) {
  int diff = mappedInGameRetarderPositions[newPos] - retarderInGamePos;
  for (int i = 0; i < diff; i++) {
    pushToButton(0);
    ++retarderInGamePos;
    if (i + 1 < diff) {
      delay(50);
    }
  }
  ++retarderCurrentPos;
}

void decreaseRetarderPos(int newPos) {
  int diff = retarderInGamePos - mappedInGameRetarderPositions[newPos];
  for (int i = 0; i < diff; i++) {
    pushToButton(1);
    --retarderInGamePos;
    if (i + 1 < diff) {
      delay(50);
    }
  }
  --retarderCurrentPos;
}

void handleShifterToggle() {
  if (shifterToggle1ReadValue != shifterToggle1State) {
    shifterToggle1State = shifterToggle1ReadValue;
    pushToButton(2);
  }
}

void handleHandBrake() {
  if (handBrakeReadValue != handBrakeState) {
    handBrakeState = handBrakeReadValue;
    pushToButton(3);
  }
}

void pushToButton(int buttonNum) {
  Joystick.pressButton(buttonNum);
  delay(10);
  Joystick.releaseButton(buttonNum);
}
