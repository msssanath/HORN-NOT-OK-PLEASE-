/**
 *NodeMCU_Thingspeak_ArduinoCore_GET_feed.ino
 * This code will first connect with your local WiFi and then will connect to your thingspeak
 * account gets the final values of three fields and based on that it will turn the GPIOs on/off. 
 * If the final field value = 1 it turns OFF, else if =2 GPIO turns ON
 * 
 * Tutorial:
 * Step1: 
 * Create a thingspeak account
 * Step3: 
 * Create a thingspeak channel with three fields
 * Step4: 
 * Update this code with WiFi username and password
 * Step4:
 * Update this code with Channel no and Read api KEY
 * Step5:
 * Upload the program into nodeMCU
 * Step6:
 * open a webbrowser and update the field values using URL provided in thingspeak
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

int num_Measure = 256 ;                           // Set the number of measurements   
int pinSignal = A0;                               // pin connected to pin O module sound sensor   
long Sound_signal;                                // Store the value read Sound Sensor   
long sum = 0 ;                                    // Store the total value of n measurements   
long level = 0 ;    
ESP8266WiFiMulti WiFiMulti;

void setup() {
   pinMode (pinSignal, INPUT);                     // Set the signal pin as input   
   Serial.begin (9600);
   pinMode(4,OUTPUT);   
    
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
    // Performs 256 signal readings   
    Serial.println("Reading signals ");
  for ( int i = 0 ; i <num_Measure; i ++)  
  {  
   Sound_signal = analogRead (pinSignal);  
    sum =sum + Sound_signal;  
    Serial.println(Sound_signal);
 
  }  
 
  level = sum / num_Measure;                       // Calculate the average value   
  Serial.print("Sound Level: ");
  Serial.println (level);  
  if(level > 0 && level <500)
  {
    //float voltage =level*(10.0/1023.0);
    analogWrite(4,20);
    delay(8000);
  }
  else if(level >=550 && level < 650)
  {
   //float voltage =level*(3.0/1023.0);
    analogWrite(4,10);
    delay(8000);
  }
  else
  {
    analogWrite(4,3);
    delay(8000);
  }
   // Reset the sum of the measurement values  
 // delay(3000);
  
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;
        String level1=(String)level;
        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("http://api.thingspeak.com/update?api_key=FGTQEXLV6CM75MCC&field1="+level1); //Update your  Channel number, KEY here
         //http.begin("http://api.thingspeak.com/channels/255189/feeds.json?results=1"); //Update your  Channel number, KEY here

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
                
                int index = payload.indexOf("field1",160); /*indexOf() Locates a character or String within another String.
                                                             By default, searches from the beginning of the String, but can also start from a given index,
                                                             allowing for the locating of all instances of the character or Str */
                USE_SERIAL.println(index);
                String newpayload = payload.substring(index+9);
                String finalnumber= newpayload.substring(0, newpayload.indexOf("\"")); /*The String function substring() is closely related to charAt(), startsWith() and endsWith().
                                                                                         It allows you to look for an instance of a particular substring within a given String. */
                USE_SERIAL.println(newpayload);
                int finalIntNumber= finalnumber.toInt();
                USE_SERIAL.println(finalIntNumber);
                if (finalIntNumber==500)
                {
                  digitalWrite(4, HIGH);
                }
                else if(finalIntNumber==0) 
                {
                  digitalWrite(4, LOW);
                }
                
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }

    delay(10000);
    sum = 0 ;
}

