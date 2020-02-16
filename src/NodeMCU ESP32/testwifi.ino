#include "MPU9250.h"
#include <Bounce2.h> // Used for "debouncing" the pushbutton
#include <WiFi.h> // Enables the ESP32 to connect to the local network (via WiFi)
#include <PubSubClient.h> // Allows us to connect to, and publish to the MQTT broker
#include <sstream>

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire,0x68);
int status;

const int buttonPin = 18; // Connect your button to pin #18

// WiFi
// Make sure to update this for your own WiFi network!
const char* ssid = "agratton";
const char* wifi_password = "monkeymerge";

// MQTT
// Make sure to update this for your own MQTT Broker!
const char* mqtt_server = "192.168.137.69";
const char* mqtt_topic = "gesture";
const char* mqtt_username = "agratton";
const char* mqtt_password = "monkeymerge";
// The client id identifies the ESP32 device. Think of it a bit like a hostname (Or just a name, like Greg).
const char* clientID = "ESP32test";

// Initialise the Pushbutton Bouncer object
// Bounce bouncer = Bounce();

// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient); // 1883 is the listener port for the Broker

void setup() {
  // serial to display data
  Serial.begin(115200);
  while(!Serial) {}

  // start communication with IMU 
  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }
  
  pinMode(buttonPin, INPUT);

  // Setup pushbutton Bouncer object
  // bouncer.attach(buttonPin);
  // bouncer.interval(5);

  // Begin Serial on 115200
  // Remember to choose the correct Baudrate on the Serial monitor!
  // This is just for debugging purposes
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Connect to the WiFi
  WiFi.begin(ssid, wifi_password);

  // Wait until the connection has been confirmed before continuing
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Debugging - Output the IP Address of the ESP8266
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to MQTT Broker
  // client.connect returns a boolean value to let us know if the connection was successful.
  // If the connection is failing, make sure you are using the correct MQTT Username and Password (Setup Earlier in the Instructable)
  if (client.connect(clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }

  pinMode(buttonPin, INPUT);
}

bool prevButtonPressed = false;

void loop() {
  // Update button state
  // This needs to be called so that the Bouncer object can check if the button has been pressed
  //bouncer.update();
  // read the sensor
  IMU.readSensor();
  
  //if (bouncer.rose()) {
  if (digitalRead(buttonPin) == HIGH) {
    // Turn light on when button is pressed down
    // (i.e. if the state of the button rose from 0 to 1 (not pressed to pressed))
    prevButtonPressed = true;

    // PUBLISH to the MQTT Broker (topic = mqtt_topic, defined at the beginning)
    // Here, "Button pressed!" is the Payload, but this could be changed to a sensor reading, for example.
    float AccelXflt = IMU.getAccelX_mss();
    float AccelYflt = IMU.getAccelY_mss();
    float AccelZflt = IMU.getAccelZ_mss();
    float GyroXflt = IMU.getGyroX_rads();
    float GyroYflt = IMU.getGyroY_rads();
    float GyroZflt = IMU.getGyroZ_rads();
    float MagXflt = IMU.getMagX_uT();
    float MagYflt = IMU.getMagY_uT();
    float MagZflt = IMU.getMagZ_uT();
    
    char Combined[144];

    int ret10 = snprintf(Combined, sizeof Combined, "%f, %f, %f, %f, %f, %f, %f, %f, %f,", AccelXflt, AccelYflt, AccelZflt, GyroXflt, GyroYflt, GyroZflt, MagXflt, MagYflt, MagZflt);
    /*
    client.publish(mqtt_topic, AccelX);
    client.publish(mqtt_topic, AccelY);
    client.publish(mqtt_topic, AccelZ);
    client.publish(mqtt_topic, GyroX);
    client.publish(mqtt_topic, GyroY);
    client.publish(mqtt_topic, GyroZ);
    client.publish(mqtt_topic, MagX);
    client.publish(mqtt_topic, MagY);
    client.publish(mqtt_topic, MagZ);
    */
    
    if (client.publish(mqtt_topic, Combined)) {
      //Serial.println("Button pushed and message sent!");
      Serial.print(AccelXflt, 6);
      Serial.print(", ");
      Serial.print(AccelYflt, 6);
      Serial.print(", ");
      Serial.print(AccelZflt, 6);
      Serial.print(", ");
      Serial.print(GyroXflt, 6);
      Serial.print(", ");
      Serial.print(GyroYflt, 6);
      Serial.print(", ");
      Serial.print(GyroZflt, 6);
      Serial.print(", ");
      Serial.print(MagXflt, 6);
      Serial.print(", ");
      Serial.print(MagYflt, 6);
      Serial.print(", ");
      Serial.print(MagZflt, 6);
      Serial.print(", ");
    }
    // Again, client.publish will return a boolean value depending on whether it succeded or not.
    // If the message failed to send, we will try again, as the connection may have broken.
    
    else {
      Serial.println("Message failed to send. Reconnecting to MQTT Broker and trying again");
      client.connect(clientID, mqtt_username, mqtt_password);
      delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
      client.publish(mqtt_topic, Combined);
      /*
      client.publish(mqtt_topic, AccelX);
      client.publish(mqtt_topic, AccelY);
      client.publish(mqtt_topic, AccelZ);
      client.publish(mqtt_topic, GyroX);
      client.publish(mqtt_topic, GyroY);
      client.publish(mqtt_topic, GyroZ);
      client.publish(mqtt_topic, MagX);
      client.publish(mqtt_topic, MagY);
      client.publish(mqtt_topic, MagZ);
      */
    }
  }
  //else if (bouncer.fell()) {
  else if (digitalRead(buttonPin) == LOW) {
    // Turn light off when button is released
    // i.e. if state goes from high (1) to low (0) (pressed to not pressed)
    Serial.print("\n");
    if (prevButtonPressed) {
      client.publish(mqtt_topic, "end");
      prevButtonPressed = false;
    }
  }
  delay(25);
}
