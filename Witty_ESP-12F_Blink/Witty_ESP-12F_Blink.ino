void setup() {
  // initialize digital pin 13 as an output.
  pinMode(15, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(15, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(15, LOW);    // turn the LED off by making the voltage LOW
  delay(500);              // wait for a second
}
