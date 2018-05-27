
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>



char auth[] = "blynk_token";
char ssid[] = "wifi_name";
char pass[] = "wifi_pass";

bool sensor0 = false;
unsigned long CurrentTime;
unsigned long StartTime;
int t = 0;
int threshold = 800;

BlynkTimer timer;
WidgetLED led1(V0);
WidgetLED led2(V1);

void checkSensors();

void setup()
{
  
  pinMode(D0, INPUT);  // input from fire detector
  pinMode(D1, OUTPUT); // LED control
  pinMode(D2, OUTPUT); // enable relay by default
  pinMode(D3, OUTPUT); // speaker control
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(100L, checkSensors);
  led1.on();
  led2.off();
}

BLYNK_CONNECTED() {
  Blynk.notify("Device started");  
}

BLYNK_WRITE(V2)
{
  threshold = param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
}


void checkSensors(){
  
  int sensorValueDigital = digitalRead(D0);
  int sensorValue = analogRead(A0);

  Serial.printf("MQ2: %d IR: %d THRES: %d\n",sensorValue, sensorValueDigital, threshold);
  
  if (( sensorValue > threshold || sensorValueDigital == 0 ) && sensor0 == false) {
    sensor0 = true;
    StartTime = millis();
  }
  
  else if (( sensorValue > threshold || sensorValueDigital == 0 ) && sensor0 == true) {
    CurrentTime = millis();
    if (CurrentTime-StartTime > 1000){
      digitalWrite(D1, HIGH);
      digitalWrite(D3, HIGH);
      led2.on();
    }
    if (CurrentTime-StartTime > 5000){
      pinMode(D2, INPUT);
      Blynk.notify("5 seconds of fire! zomg!!!");
      led1.off();
      
    }
  }
  
  else {
    sensor0 = false;
    digitalWrite(D1, LOW);  
    digitalWrite(D3, LOW);
    pinMode(D2, OUTPUT);
    led1.on();
    led2.off();
  }
}



void loop()
{
  Blynk.run();
  timer.run();
  
}


