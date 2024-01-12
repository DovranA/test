#include <ArduinoJson.h>

#include <SocketIoClient.h>

#include<Arduino.h>
//#include<ESP8266WiFi.h>
#include <WiFiManager.h>



#define USER_SERIAL Serial

 const char* ssid = "Akylly_Ahal";
 const char* pass = "ahal@!85";

const byte bulb = 16; //id_device 1
const byte pir = 4;
int pir_val = 0;
//const byte pin3 = 3;
//const byte pin4 = 4;
//const byte pin5 = 5;


 SocketIoClient webSocket;

void setup() {
  // put your setup code here, to run once:
  USER_SERIAL.begin(115200);

  pinMode(pir, INPUT_PULLUP);
  pinMode(bulb, OUTPUT);
  digitalWrite(bulb, LOW);
    
WiFiManager wm;

 bool res;
     res = wm.autoConnect(); // auto generated AP name from chipid
//     res = wm.autoConnect("AutoConnectAP"); // anonymous ap
//    res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }

  webSocket.begin("192.168.1.194",8000);
  webSocket.on("receive_device", controlled);
//  webSocket.emit("jsonObject", "{\"foo\":\"bar\"}");
}
//bool devB = true;
//bool pirB = true;
bool bulbS = true;
void loop() {
    pir_val = digitalRead(pir);
    webSocket.loop();
  
  if (pir_val == 1) {
    digitalWrite(bulb, HIGH);
    USER_SERIAL.print("pir_val: ");
    USER_SERIAL.println(pir_val);
    bulbS = true;
//    pirB = false;
//    devB = true;
    }
  if(pir_val == 0 && bulbS){
    digitalWrite(bulb, LOW);
    USER_SERIAL.print("pir_val: ");
    USER_SERIAL.println(pir_val);
    bulbS = false;
//    pirB = true;
//    devB = false;
  }
}

void controlled(const char* device, size_t length){
//    USER_SERIAL.print(message);
      DynamicJsonDocument doc(1024);
  deserializeJson(doc, device);

      int idDevice = doc["id_device"];
      int deviceState = doc["device_state"];
USER_SERIAL.print("id_device: ");
USER_SERIAL.println(idDevice);
USER_SERIAL.print("device_state: ");
USER_SERIAL.println(deviceState);

if(idDevice == 2){
  if(deviceState == 1){
    digitalWrite(bulb, HIGH);
    USER_SERIAL.print("DevS: ");
    USER_SERIAL.println(deviceState);
//    pirB = true;
//    devB = false;
bulbS = true;
    }
  if(deviceState == 0 && bulbS ){
    digitalWrite(bulb, LOW);
    USER_SERIAL.print("DevS: ");
    USER_SERIAL.println(deviceState);
//    pirB = false;
//    devB = true;

  }
}
}
