#define pirPin 2
#define ledPin 13

int val = 0;
bool motionState = false;

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(pirPin, INPUT);
    Serial.begin(9600);
}

void loop() {
    val = digitalRead(pirPin);

    if (val == HIGH) {
        digitalWrite(ledPin, HIGH);  // onboard LED

        if (motionState == false) {
            Serial.println("Motion detected!");
            motionState = true;
        }
    } else {
        digitalWrite(ledPin, LOW);
        if (motionState == true) {
            Serial.println("Motion ended!");
            motionState = false;
        }
    }
}