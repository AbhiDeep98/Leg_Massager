

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
//#define LED D0
#define LED1 D1
int timer =0;
int a = 0;
const char* ssid = "Akshaya Automations";
const char* password = "Akshay@iF";
const char* mqtt_server = "192.168.29.124";
//http://123.201.150.235:18083/
char data[80];
char inData[80];



WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50]; 
int value = 0;

//JsonObject& root = jsonBuffer.parseObject(inData);  
void setup()
{
 
 // pinMode(LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(LED1, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}


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


void callback(char* topic, byte* payload, unsigned int length) {

  char val;
  
  Serial.print("Message arrived [");
  Serial.print(topic); 
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    
  }
  Serial.println();
  StaticJsonDocument<200> jsonBuffer;
 // auto root = deserializeJson(jsonBuffer, payload);  
  DeserializationError error = deserializeJson(jsonBuffer, payload);
 
    long sec = jsonBuffer["sec"];
    long power = jsonBuffer["power"];
      Serial.print("power");
      Serial.println(power);
      Serial.print("time:");
      Serial.println(sec);
      if (a==0)
       {
        timer = sec;
        a=1;
        }
    
    if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
    }


  //d=(char)payload[0];
  // Switch on the LED if an 1 was received as first character
  if (power==1 && timer >= 0) {
    Serial.print("timer:");
    Serial.println(timer);
   // if(power == 1 && a >0){
      timer *=1000;
      Serial.println("timer started");
      //Serial.println(power); 
      Serial.println("Delay on");
      digitalWrite(LED1, HIGH);   // Turn the LED on (Note that LOW is the voltage level
  delay(1000);
  digitalWrite(LED1, LOW);
      
      delay(timer);
      digitalWrite(LED1, HIGH);   // Turn the LED on (Note that LOW is the voltage level
  delay(1000);
      digitalWrite(LED1, LOW);
      Serial.println("reset");
      a=0;
      // but actually the LED is on; this is because
      // it is active low on the ESP-01)
  
  }
  /*
  if ((char)payload[0] == '2') {
    digitalWrite(LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  }
  if((char)payload[0] == '1'){
    digitalWrite(LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
  
  if((char)payload[0] == '3'){
    //d=(char)payload[0];
    digitalWrite(LED, HIGH);
    Serial.println("LED OFF");
    delay(5000);
    digitalWrite(LED, LOW);
    Serial.println("LED ON");
  }
  int d=(char)payload[0];
  if((char)payload[0] == ' '){
    d=' ';
    digitalWrite(LED, HIGH);
    delay(d);
    digitalWrite(LED, LOW);
    
  }*/

}



void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    //snprintf (msg, 50, "hello world #%ld", value);
    //Serial.print("Publish message: ");
    //Serial.println(msg);
   // client.publish("outTopic", msg);
  }
   
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
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("InTopic");
    } else {  
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
