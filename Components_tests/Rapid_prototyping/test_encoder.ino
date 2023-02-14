int encoderPinA = 2;
int encoderPinB = 3;
int encoderPos = 0;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

bool changed = false;

void setup() {
  Serial.begin(9600);
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), updateEncoder, CHANGE);
}

void loop() {
  if(changed){
    changed = false;
    Serial.println(encoderPos);
  }
}

void updateEncoder() {
  int encoderValue = digitalRead(encoderPinA) | (digitalRead(encoderPinB) << 1);
  unsigned long currentTime = millis();
  
  if ((currentTime - lastDebounceTime) > debounceDelay) {
    switch(encoderValue) {
      case 0b00:
        break;
      case 0b01:
        encoderPos--;
        changed = true;
        break;
      case 0b10:
        encoderPos++;
        changed = true;
        break;
      case 0b11:
        break;
    }
    
    lastDebounceTime = currentTime;
  }
}
