/*
 # CONNECTIONS:
 ==============
  HCSR04)))))))) |Obstacle|
  HCSR04 ((((((((|Obstacle| 
  
  The circuit:
	[i] GRND connection of HCSR04 attached to digital pin 7 or GND.
	[i] ECHO connection of HCSR04 attached to digital pin 6.
  [i] TRIG connection of HCSR04 attached to digital pin 5
	[i] VCC connection of HCSR04 attached to digital pin 4 or +5 V.

  created 3 Nov 2008
  by David A. Mellis
  modified 30 Aug 2011
  by Tom Igoe
  modified by S. Haseeb
  14 July, 2018

  # SESNOSR OUTPUT:
  =================
  1) Pulse Duration in milli seconds.
  2) Distance in centimeters/inches.
  3) Speed of sound.
  4) Temprature.
  
  
*/

// Speed of sound at 25 degree C.
const int V_NOT = 322;
// this constant won't change. It's the pin number of the sensor's output:
const int triggerPin = 5;
const int echoPin = 6;
const int GND = 7;
const int VCC = 4;
const int speakerPin = 2;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  pinMode(triggerPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Make pin 7 GND and pin 4 VCC so the sensor could be mounted directly
  // on Arduino.
  pinMode(GND, OUTPUT);
  pinMode(VCC, OUTPUT);
  digitalWrite(GND,LOW);
  digitalWrite(VCC, HIGH);
}

void loop() {
  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:
  long duration, inches, cm;
  double c_sound, temp;

  // The HCSR04 is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(triggerPin, LOW);

  // The echo pin is used to read the signal from the HCSR04 a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  
  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  c_sound = toSpeedOfSound(duration,cm);
  temp = toTemperature(c_sound);
  tone(speakerPin, inches,20);
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm, ");
  Serial.print(c_sound);
  Serial.print("m/s, ");
  Serial.print(temp);
  Serial.print("C ");
  Serial.println();

  delay(100);
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}

double toSpeedOfSound(long microseconds, long centimeters){
  // Convert distance and time to speed
  // v = s/t
  return (centimeters*0.01)/(microseconds*0.000001);
}

double toTemperature(double speedofsound){
  // Convert speed of sound to temperature using  V = V_NOT (1 + 0.61*(T2-T1))) 
  return ( ((speedofsound-V_NOT)/(V_NOT))*(1.639344262295082) ) + 25;
}

