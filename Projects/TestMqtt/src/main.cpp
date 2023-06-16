
//ItKindaWorks - Creative Commons 2016
//github.com/ItKindaWorks
//
//Requires PubSubClient found here: https://github.com/knolleary/pubsubclient
//
//ESP8266 Simple MQTT light controller

//#include <Arduino.h>
#include <DHT.h>
//#include <DHT_U.h>
//#include <Adafruit_Sensor.h>
#include <PubSubClient.h>
#include <ESP8266Wifi.h>


//EDIT THESE LINES TO MATCH YOUR SETUP
#define DHTPIN 12 
#define DHTTYPE DHT22 

char ssid[] = "Guifi";
char password[] = "siteconectasmarchas";
IPAddress mqttServer (183,153,0,112);
const int mqttPort = 1883;
const char* mqttUser = "mauro";
const char* mqttPassword = "moscoraspi4";
const char* clientID = "NoDeMCU";
float celsiusTemperature;
char t[2];
float hum;
char h[2];
DHT dht(DHTPIN, DHTTYPE);



//void callback(char* topic, byte* payload, unsigned int length);
WiFiClient wifiClient;
PubSubClient client;

//sensor_t sensor;




//generate unique name from MAC addr
String macToStr(const uint8_t* mac){

  String result;

  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);

    if (i < 5){
      result += ':';
    }
  }
    return result;
}

/*void reconnect() {

	//attempt to connect to the wifi if connection is lost
	if(WiFi.status() != WL_CONNECTED){
		//debug printing
		Serial.print("Connecting to ");
		Serial.println(ssid);

		//loop while we wait for connection
		while (WiFi.status() != WL_CONNECTED) {
			delay(500);
			Serial.print(".");
		}

		//print out some more debug once connected
		Serial.println("");
		Serial.println("WiFi connected");  
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());
	}

	//make sure we are connected to WIFI before attemping to reconnect to MQTT
	if(WiFi.status() == WL_CONNECTED){
	// Loop until we're reconnected to the MQTT server
		while (!client.connected()) {
			Serial.print("Attempting MQTT connection...");

			// Generate client name based on MAC address and last 8 bits of microsecond counter
			String clientName;
			clientName = "esp8266";
			Serial.println(clientName);
			//uint8_t mac[6];
			//WiFi.macAddress(mac);
			//clientName += macToStr(mac);

			//if connected, subscribe to the topic(s) we want to be notified about
			if (client.connect((char*) clientName.c_str())) {
				Serial.print("\tMTQQ Connected");
				client.subscribe("/test/tempyhum");
			}

			//otherwise print failed for debugging
			else{Serial.println("\tFailed."); abort();}
		}
	}
}*/


void setup() {
	
   Serial.begin(115200);
	delay(100);
  	dht.begin();
	WiFi.mode(WIFI_STA);
  	WiFi.begin(ssid, password);
 

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando a red WiFi...");
  }
  	WiFi.printDiag(Serial);
	Serial.println(WiFi.localIP());

	

	client.setClient(wifiClient);
	//client.setCallback(callback);
	client.setServer(mqttServer, mqttPort);

	while (!client.connected()) {
    Serial.println("Conectando a Broquer MQTT...");
	Serial.println(clientID);
	Serial.println("");
	//Serial.println(mqttUser);
	//Serial.println(mqttPassword);
	Serial.println(mqttServer);
	Serial.println("");
	Serial.println(mqttPort);
 
    if (client.connect(clientID, mqttUser, mqttPassword)) {
 
      Serial.println("conectado");
 
    } else {
 
      Serial.println("conexion fallida ");
      Serial.println(client.state());
	  Serial.println("");
	 // WiFi.printDiag(Serial);
	  Serial.println(WiFi.localIP());
	  Serial.println("");
	  
       
    }
  }
    delay(1000);

}



void loop(){

	//sensors_event_t event;
	celsiusTemperature = dht.readTemperature(false);
	hum = dht.readHumidity();

	//reconnect if connection is lost
	/*if (!client.connected() && WiFi.status() == 3) {
		connectD();
		}*/
 	//dht.temperature().getEvent(&event);
	if (isnan(celsiusTemperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
	  int  temp = ((int)celsiusTemperature);
    Serial.print("Temperature: ");
    Serial.print(temp);
	Serial.print(" ");
    Serial.println("C");
	String b;
	b = (String)temp;
	b.toCharArray(t,temp);
		
  }

  if (isnan(hum)) {
    Serial.println("Error reading humedad!");
  }
  else {
	  int  humus = ((int)hum);
    Serial.print("Humedad: ");
    Serial.print(humus);
	Serial.print(" ");
    Serial.println("%");
	String b;
	b = (String)humus;
	b.toCharArray(h,humus);
		
  }
client.publish("/test/tempyhum", "Temperatura:");
	client.publish("/test/tempyhum", t);
	
	client.publish("/test/tempyhum", "Humedad");
	client.publish("/test/tempyhum", h);
	
	client.publish("/test/tempyhum", " ");


	//maintain MQTT connection
	delay(1000);
	//client.loop();
	
 
}

/*void callback (char* topic, byte* payload, unsigned int length) {

	//convert topic to string to make it easier to work with
	String topicStr = topic; 
	sensors_event_t event;
	//Print out some debugging info
	Serial.println("Callback update.");
	Serial.print("Topic: ");
	Serial.println(topicStr);

	//turn the light on if the payload is '1' and publish to the MQTT server a confirmation message
	if(payload[0] == '1'){
		//digitalWrite(lightPin, HIGH);
		
		if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
	t = event.temperature;
	int j = int(t);
	temp += j;
  }

		client.publish("/test/tempyhum", temp);

	}

	//turn the light off if the payload is '0' and publish to the MQTT server a confirmation message
	if (payload[0] == '0'){
		//digitalWrite(lightPin, LOW);
		if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
	h=event.relative_humidity;
	int i = int(h);
	hum += i;
  }

		client.publish("/test/tempyhum", hum);
	}

}*/
void connectD(){

	while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando a red WiFi...");
  }
 
  Serial.println("Conectado a la red WiFi");

	//start the serial line for debugging
	Serial.begin(115200);
	delay(100);

	client.setClient(wifiClient);

	client.setServer(mqttServer, mqttPort);

	while (!client.connected()) {
    Serial.println("Conectando a Broquer MQTT...");
 
    if (client.connect("IOT-ESP32", mqttUser, mqttPassword )) {
 
      Serial.println("conectado");
 
    } else {
 
      Serial.print("conexion fallida ");
      Serial.print(client.state());
      delay(2000);
 
    }
	}
}

