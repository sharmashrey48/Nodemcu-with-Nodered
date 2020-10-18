#include <ESP8266WiFi.h>
#include <PubSubClient.h>  //callback    //reconnect 
#include "DHT.h"
#define DHTPIN D2   
#define DHTTYPE DHT11  
int pstate1 = LOW;

 int flag1 = 0; 

DHT dht(DHTPIN, DHTTYPE);
// Update these with values suitable for your network.

const char* ssid = "Does it matter?";          // SSID for your wifi Router
const char* passkey = "qwertyuiop";  //Password for wifi Router

const char* mqtt_server = "192.168.2.10" ; //mqtt server domain or IP
const char* topic = "livingroom";    // topic for mqtt

//const char* username = "shrey";   //username for mqtt broker
//const char* password = "qwertyuiop";  // password for mqtt broker



WiFiClient espClient;
PubSubClient client(espClient);


void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, passkey);

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

void callback(char* topic, byte* payload, unsigned int length) {
  String topicstr = topic;
   char sub[1000];
   int c =0;
  Serial.print("Message arrived [");        // Message Arrived [test] a
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();


  if ((char)payload[0] == 'b') {
    digitalWrite(D4, LOW); 
    flag1 = 0;
    //client.publish("testing/publish","a",false);
   // Serial.println("D4 is OFF");  
  }if ((char)payload[0] == 'a'){
    digitalWrite(D4, HIGH); 
    flag1 = 1;
    //Serial.println("D4 is ON");
    //client.publish("testing/publish","b",false);
  }if ((char)payload[0] == 'c') {
    digitalWrite(D6, LOW); 
   // client.publish("testingpub","c",false);  
  }if ((char)payload[0] == 'd'){
    digitalWrite(D6, HIGH);  
    //client.publish("testingpub","d",false);
  }if ((char)payload[0] == 'e') {
    digitalWrite(D7, LOW);  
    //client.publish("testingpub","e",false);
  }if ((char)payload[0] == 'f'){
    digitalWrite(D7, HIGH); 
    //client.publish("testingpub","f",false); 
  }if ((char)payload[0] == 'g') {
    digitalWrite(D3, LOW);  
   // client.publish("testingpub","g",true);
  }if ((char)payload[0] == 'h'){
    digitalWrite(D3, HIGH); 
   // client.publish("testingpub","h",false);
  }




}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
      String clientId = "Nodemcu";
    // Attempt to connect
    if (client.connect(clientId.c_str())) { // will message 
       Serial.println("connected");
       client.publish("notifications","Home is online",true);
       client.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(D4, OUTPUT); 
  pinMode(D6, OUTPUT); 
  pinMode(D7, INPUT);
  pinMode(D5, INPUT);
  pinMode(D3, OUTPUT);
  //pinMode(D0, INPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server,1883);
  client.setCallback(callback);
   dht.begin();
}

void loop() {

   pstate1= readdata(D5, D4, pstate1);
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
////======================================read state of GPIO input pin ==================================================
int readdata(uint8_t Rpin, uint8_t Epin , int pstate){
   int cstate = digitalRead(Rpin);
  if(pstate != cstate){
    if(cstate == HIGH){
       
        digitalWrite(Epin, HIGH);
        client.publish("publish","a",true);
       Serial.println("a");
    } 
    else if(cstate==LOW){
 
        digitalWrite(Epin,LOW);
         client.publish("publish","b",true);
        Serial.println("b");
    }
  }return cstate;
}
