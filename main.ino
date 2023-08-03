#define BLYNK_TEMPLATE_ID "TMPL3bMRtL3e5"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN
"3-G9wH7mwsgpQkqPJ_wER4SVwo9kieXa"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
int pos = 0;
// Your WiFi credentials.
char ssid[] = "Wifi_SSID";
char pass[] = "wifi_password";
BlynkTimer timer;
const int gasPin=34;
const int trigPin = 32;
const int echoPin = 33;
const int motionPin = 25;
const int lightDetection = 26;
const int servoPin=18;
const int LedPin=19;
const int Buzzer=14;
long duration,containerHeight=24.0;
int distance;
int motion,warm_up;
int ledMode=1;
int ledControl=0;
int LDR_Reading=0;
Servo myservo;
// This function is called every time the Virtual Pin 0 state
changes
void updateLED(){
if((ledMode&&LDR_Reading)||(!ledMode&&ledControl)){
digitalWrite(LedPin,HIGH);
Blynk.virtualWrite(V9,1);
}
else{digitalWrite(LedPin,LOW);Blynk.virtualWrite(V9,0);}
}
BLYNK_WRITE(V8)
{
// Set incoming value from pin V8 to a variable
ledMode = param.asInt();
updateLED();
}
BLYNK_WRITE(V7)
{
// Set incoming value from pin V7 to a variable
ledControl = param.asInt();
updateLED();
}
// This function is called every time the device is connected
to the Blynk.Cloud
BLYNK_CONNECTED()
{
Serial.println("Connected to blynk");
}
int readUltrasonicDistance()
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in
microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance = duration * 0.034 / 2;
return distance;
}
// This function is called every 1 second
void myTimerEvent()
{
float h = analogRead(gasPin);
if (isnan(h))
{
Serial.println("Failed to read from MQ-5 sensor!");
return;
}
float gasValue=(h/4095)*100;
int water_level=
((containerHeight-readUltrasonicDistance())*100.0)/containerHei
ght;
Blynk.virtualWrite(V3, gasValue);
Blynk.virtualWrite(V4,water_level);
LDR_Reading = digitalRead(lightDetection);
Blynk.virtualWrite(V6,LDR_Reading);
updateLED();
if(gasValue>60){tone(Buzzer,900);}else{noTone(Buzzer);}
}
//coding the servo based on motion sensor values
void motionDetect(){
motion = digitalRead(motionPin);
int x=myservo.read();
if( motion == LOW )
{
Serial.print("No object in sight\n\n");
if(x>0){
for(int i=x;i>0;i=i-2){
myservo.write(i);
delay(50);
}}
Blynk.virtualWrite(V5,0);
}else
{
Serial.print("Object detected\n\n");
Blynk.virtualWrite(V5,1);
if(x<90){
for(int i=x;i<=90;i=i+5){
myservo.write(i);
delay(50);
}
}
}
}
void setup()
{
Serial.begin(115200);
Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
// You can also specify server:
//Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud",
80);
//Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass,
IPAddress(192,168,1,100), 8080);
// Setup a function to be called every second
timer.setInterval(1000L, myTimerEvent);
//function called every 5 seconds
timer.setInterval(5000L,motionDetect);
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT);
pinMode(motionPin,INPUT);
pinMode(lightDetection,INPUT);
pinMode(servoPin,OUTPUT);
pinMode(LedPin,OUTPUT);
pinMode(Buzzer,OUTPUT);
myservo.attach(servoPin);
myservo.write(0);
}
void loop()
{
Blynk.run();
timer.run();
}
