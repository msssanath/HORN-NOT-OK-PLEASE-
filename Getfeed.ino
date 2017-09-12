/**
 *NodeMCU_Thingspeak_ArduinoCore_GET_feed.ino
 * This code will first connect with your local WiFi and then will connect to your thingspeak
 * account gets the final values of three fields and based on that it will turn the GPIOs on/off. 
 * If the final field value = 1 it turns OFF, else if =2 GPIO turns ON
 * 
 * Tutorial:
 * Step1: 
 * Create a thingspeak account
 * Step2: 
 * Create a thingspeak channel with three fields
 * Step3: 
 * Update this code with WiFi username and password
 * Step4:
 * Update this code with Channel no and Read api KEY
 * Step5:
 * Upload the program into nodeMCU
 * Step6:
 * Open a webbrowser and update the field values using URL provided in thingspeak
 * Step7: 
 * Observe the changes by connecting LEDs to Nodemcu D1, D2, D4 pins
 * 
 * field1 : GPIO2 : D4
 * feild2 : GPIO4 : D2
 * field3 : GPIO5 : D1
 * 
 *  Created on: 4-Apr-2017
 *  By Himesh Reddivari @ www.module143.com
 *  email: himesh@module143.com / himesh1729@gmail.com
 *  Ph : +91 7032366913
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial
String level= "500";
ESP8266WiFiMulti WiFiMulti;

void setup() {
  
    pinMode(4, OUTPUT);
    
    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("psychlopedia", "pawancr7");    // *******Change your Username and Password of wifi here

}

void loop() {
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        //http.begin("http://api.thingspeak.com/update?api_key=FGTQEXLV6CM75MCC&field1="+level); //Update your  Channel number, KEY here
         http.begin("http://api.thingspeak.com/channels/255189/feeds.json?results=1"); //Update your  Channel number, KEY here

        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println("I am here");
                USE_SERIAL.println(payload);
                //int index = payload.indexOf("field1");
                //USE_SERIAL.println(index);
                
                int index = payload.indexOf("field1",160);
                USE_SERIAL.println(index);
                String newpayload = payload.substring(index+9);
                String finalnumber= newpayload.substring(0, newpayload.indexOf("\""));
                USE_SERIAL.println(newpayload);
                int finalIntNumber= finalnumber.toInt();
                USE_SERIAL.println(finalIntNumber);
                

                 if(finalIntNumber > 0 && finalIntNumber <500)
                   {
                    
                    analogWrite(4,20); // Writes an Analog value to a pin
                                       // Writes to pin 4 with a duty cycle of 20
                    delay(3000);
                   }
                 else if(finalIntNumber >=500 && finalIntNumber < 650)
                  {
                   
                     analogWrite(4,10);
                      delay(3000);
                  }
                else
                   {
                    analogWrite(4,3);
                    delay(3000);
                   }

                
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }

    delay(10000);
}

