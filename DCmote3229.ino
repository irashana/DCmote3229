#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
#include "FS.h"
#include <SPI.h>

#include <ArduinoJson.h>
#include <WiFiClient.h> 
  WiFiClient client;

const char* ssid     = "DCmoteRIO2";
const char* password = "mydcmote3228";
 int num=0;
    String result ="";
    String line ="";

const char* host     = "www.dcmote.xyz"; // Your domain  
//String path          = "/light.json";  
String path          = "/json.php?";  
//String path          = "/index.html";  
const int pin1        = D0; // gpio 16
const int pin2        = D2;
const int pin3        = D3;
const int pin4        = D4;
const int pin5        = D5;  // gpio 14
const int pin6        = D6; // gpio12
const int pin7        = D7;   // gpio 13
const int pin8        = D8;

#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

//for LED status




//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
 // ticker.attach(0.2, tick);
}

void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}




void setup() {
 SPIFFS.begin();///////////////spiffs
      
     
  
 File mydid = SPIFFS.open("/mydid.txt", "r");   
  String ddd = mydid.readStringUntil(1); 
  Serial.println(ddd);
    path += "email=";
  path += "@indom";
  path += "&did=";
 // path += "2ca63cddd54f9490efad22421891a9d1";
    path += ddd;
     
  
   pinMode(D0,OUTPUT);  
 pinMode(pin1, OUTPUT); 
   pinMode(pin2, OUTPUT); 
    pinMode(pin3, OUTPUT); 
     pinMode(pin4, OUTPUT); 
      pinMode(pin5, OUTPUT); 
       pinMode(pin6, OUTPUT); 
        pinMode(pin7, OUTPUT); 
         pinMode(pin8, OUTPUT); 
  
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  //set led pin as output
 // pinMode(BUILTIN_LED, OUTPUT);
  // start ticker with 0.5 because we start in AP mode and try to connect
  //ticker.attach(0.6, tick);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
  //wifiManager.resetSettings();

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  if (!wifiManager.autoConnect(ssid,password)) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
 // ticker.detach();
  //keep LED on
 // digitalWrite(BUILTIN_LED, LOW);


  
}


 

void loop() {


  File mydid = SPIFFS.open("/mydid.txt", "r");   
  String ddd = mydid.readStringUntil(1); 
  Serial.println(ddd);



  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  

 //client.print(String("GET ") + path + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" +  "Connection: keep-alive\r\n\r\n");
   client.print(String("GET ") + path + " HTTP/1.1\r\n" +"Host: " + host + "\r\n" + "Connection: close\r\n\r\n");


  delay(50); // wait for server to respond

  while(client.available()){
     line = client.readStringUntil('\r');  
       
  
           result = line.substring(0,40);
 Serial.println(result);
 int data1 = result.indexOf("##");
  Serial.println("The index of @ in the string " + result + " is " + data1);
 String Data =result.substring(1,data1);
 Serial.println(" Data is Tracked " + Data);

 int s1data = Data.indexOf("@1>");
  int s2data = Data.indexOf("@2>");
   int s3data = Data.indexOf("@3>");
    int s4data = Data.indexOf("@4>");
    int last =Data.indexOf('#');

    String s1= Data.substring(s1data,s2data);
     String s2= Data.substring(s2data,s3data);
      String s3= Data.substring(s3data,s4data);
       String s4= Data.substring(s4data,last);
     Serial.println(" My S1 data is  " + s1 );
      Serial.println(" My S2 data is  " + s2 );
       Serial.println(" My S3 data is  " + s3 );
        Serial.println(" My S4 data is  " + s4 );
 
           
      if (s1=="@1>on"){  Serial.println(" 1 is on");     digitalWrite(pin5,HIGH ); }
       if (s2=="@2>on"){  Serial.println(" 2 is on");   digitalWrite(pin6,HIGH ); }
       if (s3=="@3>on"){  Serial.println(" 3 is on");    digitalWrite(pin7,HIGH );}
        if (s4=="@4>on"){  Serial.println(" 4 is on");   digitalWrite(pin1,HIGH );}

          if (s1=="@1>off"){  Serial.println(" 1 is off"); digitalWrite(pin5,LOW );  }
          if (s2=="@2>off"){  Serial.println(" 2 is off"); digitalWrite(pin6,LOW );  }
          if (s3=="@3>off"){  Serial.println(" 3 is off"); digitalWrite(pin7,LOW );  }
          if (s4=="@4>off"){  Serial.println(" 4 is off"); digitalWrite(pin1,LOW );  }
   
       
  }
// num++;
//  Serial.println(num);
 // if (num>=148){ESP.reset();}
 //  if (num>=1){ //Serial.println(result); 

 //   Serial.flush();
 //     client.flush();
  //   client.stop();
  //    line ="";
//   }
    
  
}

 
 

