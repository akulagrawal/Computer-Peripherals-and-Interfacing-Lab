// defines pins numbers
const int trigPin = 49;
const int echoPin = 40;
// defines variables
long duration;
int distance;
void setup() {
//pinMode(6, OUTPUT);
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
Serial.begin(9600); // Starts the serial communication
}
void loop() {
//  digitalWrite(6, HIGH);
//  delay(analogRead(0));
//  digitalWrite(6, LOW);
//  delay(analogRead(0));
// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);

if(distance < 20)
{
  digitalWrite(6, HIGH);  
}
else
{
  digitalWrite(6, LOW);
}
delay(500);
}
