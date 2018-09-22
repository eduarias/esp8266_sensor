/*
 * Basic IO
 * Use a pushbutton to toggle the onboard LED.
 *
 * If you do not have the 1 Button Shield, add a pushbutton or switch between D3 and GND
 */

int inPin = D3;  // pushbutton connected to digital pin D3
int val = 0;        // variable to store the read value
int ledPin = D5;

void setup() {
  pinMode(ledPin, OUTPUT);  // set onboard LED as output
  pinMode(inPin, INPUT);      // set pin as input
  Serial.begin(9600);
}

void loop() {
  val = digitalRead(inPin);  // read input value
  if (val == HIGH) {         // check if the input is HIGH (button released)
    digitalWrite(ledPin, LOW);  // turn LED OFF
  } else {
    digitalWrite(ledPin, HIGH);  // turn LED ON
  }
}
