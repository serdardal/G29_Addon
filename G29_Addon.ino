int sensorValue;
int minValue = 150;
int maxValue = 500;

int pos0Value;
int pos1Value;
int pos2Value;
int pos3Value;

int currentPos = 0;

int tolerance = 10;

// the setup routine runs once when you press reset:
void setup() {
	// initialize serial communication at 9600 bits per second:
	Serial.begin(9600);
	setPosValues();
  Serial.print("Current pos: ");
  Serial.println(currentPos);
}

// the loop routine runs over and over again forever:
void loop() {
	// read the input on analog pin 0:
	sensorValue = analogRead(A0);
	// print out the value you read:
	//Serial.println(sensorValue);
  int newPos = getNewPos(sensorValue);
  if(newPos >= 0 && newPos != currentPos){
    if(newPos - currentPos > 0){
      increasePos(newPos);
    }else{
      decreasePos(newPos);
    }
  }
  
	delay(10);        // delay in between reads for stability
}

void setPosValues() {
	pos0Value = minValue;
	pos3Value = maxValue;
	
	int difference = maxValue - minValue;
	int range = difference / 3 ;

  pos1Value = minValue + range;
  pos2Value = maxValue - range;
}

int getNewPos(int value) {
  if(abs(value - pos0Value) < tolerance){
    return 0;
  }else if(abs(value - pos1Value) < tolerance){
    return 1;  
  }else if(abs(value - pos2Value) < tolerance){
    return 2;  
  }else if(abs(value - pos3Value) < tolerance){
    return 3;
  }else return -1;
}

void increasePos(int newPos) {
  int diff = newPos - currentPos;
  for(int i = 0; i < diff; i++){
      Serial.print("Position increased. Current pos: ");
      Serial.println(++currentPos);

      if(i >= 0 && i < (diff - 1)){
        Serial.println("waiting...");
        delay(1000);
      }
  }
}

void decreasePos(int newPos) {
  int diff = currentPos - newPos;
  for(int i = 0; i < diff; i++){
      Serial.print("Position decreased. Current pos: ");
      Serial.println(--currentPos);

      if(i >= 0 && i < (diff - 1)){
        Serial.println("waiting...");
        delay(1000);
      }
  }  
}
