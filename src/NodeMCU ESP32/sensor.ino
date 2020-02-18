#include "MPU9250.h"
#include <Bounce2.h> //button debouncing
#include <WiFi.h>
#include <PubSubClient.h> //MQTT client
#include <sstream>

MPU9250 IMU(Wire, 0x68);
int status;

const int buttonPin = 18; //button pin

// Wifi credentials (local only)
const char *ssid = "agratton";
const char *wifi_password = "monkeymerge";

//MQTT credentials (local only)
const char *mqtt_server = "192.168.137.69";
const char *mqtt_topic = "gesture";
const char *mqtt_username = "agratton";
const char *mqtt_password = "monkeymerge";

//Sensor device name
const char *clientID = "SensingUnit";

// Wifi and MQTT initi
WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient);

void setup() {
	// serial to display data (for debugging)
	Serial.begin(115200);
	while (!Serial) {}

	// start communication with IMU
	status = IMU.begin();
	if (status < 0) {
		Serial.println("IMU initialization unsuccessful");
		Serial.println(status);
		while (1) {}
	}

	pinMode(buttonPin, INPUT);

	//begin serial debug connection
	Serial.begin(115200);

	Serial.print("Connecting to ");
	Serial.println(ssid);

	// Connect to WiFi
	WiFi.begin(ssid, wifi_password);

	// Wait for confirmation
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	// Connect to MQTT Broker
	if (client.connect(clientID, mqtt_username, mqtt_password)) {
		Serial.println("Connected to MQTT Broker!");
	} else {
		Serial.println("Connection to MQTT Broker failed...");
	}

	pinMode(buttonPin, INPUT);
}

bool prevButtonPressed = false;

void loop() {
	// read the sensor
	IMU.readSensor();

	if (digitalRead(buttonPin) == HIGH) {

		// PUBLISH to the MQTT Broker
		float accelX = IMU.getAccelX_mss();
		float accelY = IMU.getAccelY_mss();
		float accelZ = IMU.getAccelZ_mss();
		float gyroX = IMU.getGyroX_rads();
		float gyroY = IMU.getGyroY_rads();
		float gyroZ = IMU.getGyroZ_rads();
		float MagXflt = IMU.getMagX_uT();
		float magY = IMU.getMagY_uT();
		float magZ = IMU.getMagZ_uT();

		char Combined[144];

		int ret10 = snprintf(Combined,
							 sizeof Combined,
							 "%f, %f, %f, %f, %f, %f, %f, %f, %f,",
							 accelX,
							 accelY,
							 accelZ,
							 gyroX,
							 gyroY,
							 gyroZ,
							 MagXflt,
							 magY,
							 magZ);

		if (client.publish(mqtt_topic, Combined)) {
			//debugging
			Serial.print(accelX, 6);
			Serial.print(", ");
			Serial.print(accelY, 6);
			Serial.print(", ");
			Serial.print(accelZ, 6);
			Serial.print(", ");
			Serial.print(gyroX, 6);
			Serial.print(", ");
			Serial.print(gyroY, 6);
			Serial.print(", ");
			Serial.print(gyroZ, 6);
			Serial.print(", ");
			Serial.print(magX, 6);
			Serial.print(", ");
			Serial.print(magY, 6);
			Serial.print(", ");
			Serial.print(magZ, 6);
			Serial.print(", ");
		} else {
			Serial.println("attempting reconnect");
			client.connect(clientID, mqtt_username, mqtt_password);
			delay(10); //allowing for connection
			client.publish(mqtt_topic, Combined);
		}
	} else if (digitalRead(buttonPin) == LOW) {
		Serial.print("\n");
		if (prevButtonPressed) {
			client.publish(mqtt_topic, "end"); //for determining ends of gesture inputs
			prevButtonPressed = false;
		}
	}
	delay(25); //refresh data every 25ms
}
