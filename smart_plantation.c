#include "DHT.h"  
#include <SoftwareSerial.h>
SoftwareSerial mySerial(6, 7);
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

int phValue =0;
int ldr = D0;
int Buzzer = D5;
int Gas = A0;
int Motor = D2;

 DHT dht2(2,DHT11);  

char ssid[] = "Kichu";   // your network SSID (name)
char pass[] = "10022003";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;
 
unsigned long myChannelNumber = 2491696;
const char * myWriteAPIKey = "GR3ME71XNR229O0K";


 
 void setup()  
 {  
  Serial.begin(9600); 
  mySerial.begin(9600); 
  pinMode(ldr,INPUT);
  pinMode(Buzzer,OUTPUT);
  pinMode(Gas,INPUT);
  pinMode(Motor,OUTPUT);
  
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);  
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
 }  
 
 void loop()  
 {  
  int ldr = digitalRead(D0);
  int Gas = analogRead(A0);
  String phdata = Serial.readStringUntil(':');
   
   if(phdata != "")
   {
     String ph =Serial.readStringUntil('$');
     phValue=ph.toFloat();
     } 
     
   Serial.println("Temperature in C:");  
   Serial.println((dht2.readTemperature( )));  
   Serial.println("Humidity in C:");  
   Serial.println((dht2.readHumidity()));  

   Serial.print("pH:");
   Serial.println(phValue);

   Serial.print("LDR:");
   Serial.println(ldr);

   Serial.print("Gas:");
   Serial.println(Gas);

   if ((ldr == 1)||(Gas > 1024))
   {
    digitalWrite(Motor,HIGH);
    digitalWrite(Buzzer,HIGH); 
   } 
   else
   {
    digitalWrite(Motor,LOW);
    digitalWrite(Buzzer,LOW);
   }

    ThingSpeak.setField(1, Gas);
    ThingSpeak.setField(2, ldr);
    ThingSpeak.setField(3, phValue);
    ThingSpeak.setField(4, DHT11);
 
    // write to the ThingSpeak channel
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey );
    if (x == 200)
    {
      Serial.println("Channel update successful.");
    }
    else
    {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
   delay(100);  
 }