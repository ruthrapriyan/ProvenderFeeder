#include <Servo.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#define Firebase_Host "https://iot-feeder-1d737-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define Firebase_Auth "E3ZIcq4fdZaPuihuSBX0yBLxNlpnLW5BNGwfZBYf"


const char* ssid = "kirthik";
const char* password = "987654321";
const int trigPin = D6;
const int echoPin = D7;
long duration;
int distance=0;
const int sensorPin = A0;
int ledPin = 4;


// Variables for storing the sensor data
int waterLevel = 0;
FirebaseData firebaseData;

FirebaseJson json;

Servo servo;

WiFiServer server(80);


void setup()
{
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
servo.attach(5); //D1

servo.write(0);

delay(2000);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  
  Firebase.begin(Firebase_Host, Firebase_Auth);
  Firebase.reconnectWiFi(true);
  
}
void loop()
{
  delay(1000);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // measure the duration of the echo pulse in microseconds
  duration = pulseIn(echoPin, HIGH);

  // calculate the distance in centimeters
  distance = duration*0.034/2;

  if(1){
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm     ");
  Serial.println("Food is very LOW");

  
}
waterLevel = analogRead(sensorPin)-5;

  if(waterLevel<100){
  Serial.print("Water Level: ");
  Serial.println(waterLevel);
  }

  
  delay(1000);
  Firebase.setString(firebaseData, "ultra value/us", distance);
  Firebase.setString(firebaseData, "ultra value/lvl", waterLevel);
  if(Firebase.getString(firebaseData, "/ultra value/user_name"))
  {
    String ledstatus = firebaseData.stringData();
    if(ledstatus.toInt() == 0){
      servo.write(180);
delay(2000);
servo.write(0);
delay(1000);
    }
    if(Firebase.getString(firebaseData, "/ultra value/Water_pump"))
  {
    String waterstatus = firebaseData.stringData();
    if(waterstatus.toInt() == 0){
      digitalWrite(ledPin, LOW);

    }
    else{
      digitalWrite(ledPin, HIGH);
    }

  }
  }
}