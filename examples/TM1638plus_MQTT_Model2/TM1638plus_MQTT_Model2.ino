/*
  Project Name: TM1638plus
  File: TM1638plus_MQTT_TEST_Model2.ino
  Author: wunder baum 
  Description: Example file contributed by [wunderbaum](https://github.com/wunderbaum) on github 
  TM1638 with MQTT on an ESP8266 (Wemos)
  sending buttons to topic
  receiving message to display 
  model 2 with 16 buttons used for this example.

  Created: Sep 2020
  URL: https://github.com/gavinlyonsrepo/TM1638plus
*/


// https://github.com/gavinlyonsrepo/TM1638plus
#include <TM1638plus_Model2.h>
// https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
#include <ESP8266WiFi.h>
// https://github.com/knolleary/pubsubclient
#include <PubSubClient.h>


#define  myTestDelay 5000
uint8_t  testcount = 0;
unsigned char buttonmerk;


const char* ssid = "SSID";
const char* password  = "PASSWORD";
const char* mqtt_server = "10.10.20.20";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;


// GPIO I/O pins on the Arduino connected to strobe, clock, data, pick on any I/O pin you want.
#define  STROBE_TM D1  // strobe = GPIO connected to strobe line of module
#define  CLOCK_TM D2  // clock = GPIO connected to clock line of module
#define  DIO_TM D3 // data = GPIO connected to data line of module
bool swap_nibbles = false; //Default is false if left out, see note in readme at URL
bool high_freq = false; //default false,, If using a high freq CPU > ~100 MHZ set to true. 

// Constructor object
TM1638plus_Model2 tm(STROBE_TM, CLOCK_TM , DIO_TM, swap_nibbles, high_freq);

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// This is what happens on an incoming mqtt-message
void callback(char* topic, byte* payload, unsigned int length) {
  char message_buff[100];
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    message_buff[i] = payload[i];
  }
  String msgString = String(message_buff);
  Serial.print(msgString);
  tm.DisplayStr(message_buff,0);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("TM1638/pub", "TM1638 ready.");
      // ... and resubscribe
     client.subscribe("TM1638/message");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//Function to setup serial called from setup
void Serialinit()
{
  Serial.begin(115200);
  delay(100);
  Serial.println("-------------Comms UP------------");
}


void setup() {
  Serialinit();
  tm.displayBegin(); // Init the module
  delay(myTestDelay);

 // WLAN config
 setup_wifi();

 // MQTT
 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);
  // reset display
 tm.reset();
}

void loop() {

if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(200);
  unsigned char buttons = tm.ReadKey16();
  snprintf (msg, 50, "%ld", buttons);
  if (buttons != buttonmerk & buttons != 0) {
  tm.DisplayDecNum(buttons, 0, false );
  Serial.println(msg);
  // 
   
  client.publish("TM1638/button", msg);
  }
  buttonmerk = buttons;

}
