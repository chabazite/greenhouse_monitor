

#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include "DHT.h"
#include <ESP_Mail_Client.h>

#define DHTPIN 04     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
 
#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
 
float h ;
float t;
float hif;
unsigned long epochTime;
String currentDateTime;
const int ledPin = 2; 
unsigned long lastLoopTime = 0; // Use to check if loop is running periodically

bool wifiConnected = false;
bool awsConnected = false;

DHT dht(DHTPIN, DHTTYPE);
 
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

SMTPSession smtp;
SMTP_Message message;

// Network Time Protocol (NTP) settings
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000); // 0 offset for UTC, 60000ms refresh

void smtpCallback(SMTP_Status status) {
    // This function will be called by the SMTP client to report its status.
    // You can implement logging or other actions based on the status.

    // Example: Print the status information to the Serial console.
    Serial.println(status.info());

    if (status.success()) {
        Serial.println("Email sent successfully!");
    } else {
        Serial.println("Failed to send email!");
        // You can add more detailed error handling here.
    }
}

String getFormattedDateTime() {
  timeClient.update();
  epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime((time_t *)&epochTime); 

  char dateBuffer[20];
  // Format: YYYY-MM-DD hh:mm:ss
  snprintf(dateBuffer, sizeof(dateBuffer), "%d-%02d-%02d %02d:%02d:%02d", 
           ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, 
           ptm->tm_hour, ptm->tm_min, ptm->tm_sec);

  return String(dateBuffer);
}

void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);
 
  StaticJsonDocument<400> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
}

void sendNotificationEmail(const String& subject, const String& messageContent, int retryCount = -1) {
  String fullMessage;

  if (retryCount >= 0) {
      // If retryCount is provided, include it in the message
      fullMessage = messageContent + " Attempt: " + String(retryCount);
  } else {
      // If retryCount is not provided, use the message content as is
      fullMessage = messageContent;
  }

  message.sender.name = "ESP32";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = subject;
  message.addRecipient("Recipient", RECIPIENT_EMAIL);

  message.text.content = fullMessage.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_normal;

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("Error sending Email");
  }
}

void connectToWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("Connecting to Wi-Fi");

    pinMode(ledPin, OUTPUT);



    int retryCount = 0;
    while (WiFi.status() != WL_CONNECTED && retryCount < 5) {
        sendNotificationEmail("ESP32 Alert","Trying to reconnect to Wifi", retryCount);
        delay(1000);
        Serial.print(".");
        digitalWrite(ledPin, HIGH);  // Turn on the LED
        delay(1000);
        digitalWrite(ledPin, LOW);  // Turn on the LED
        delay(1000);
        digitalWrite(ledPin, HIGH);  // Turn on the LED
        delay(1000);
        digitalWrite(ledPin, LOW);  // Turn on the LED
        delay(1000);
        retryCount++;
    }

    if (WiFi.status() != WL_CONNECTED) {
        if (wifiConnected) {  // Was connected before, now disconnected
            sendNotificationEmail("ESP32 Alert", "Failed to connect to Wi-Fi. Restarting...");
            wifiConnected = false;
        }
        Serial.println("Failed to connect to Wi-Fi. Restarting...");
        ESP.restart();
    } else {
        if (!wifiConnected) {  // Just connected
            sendNotificationEmail("ESP32 Alert", "Wi-Fi Connection Restored");
            wifiConnected = true;
        }
    }
    
    digitalWrite(ledPin, HIGH);  // Turn on the LED
    Serial.println("Connected to Wi-Fi");
    delay(2000);
}

void connectToAWSIoT() {
    // Configure WiFiClientSecure to use the AWS IoT device credentials
    net.setCACert(AWS_CERT_CA);
    net.setCertificate(AWS_CERT_CRT);
    net.setPrivateKey(AWS_CERT_PRIVATE);

    // Connect to the MQTT broker on the AWS endpoint we defined earlier
    client.setServer(AWS_IOT_ENDPOINT, 8883);

    int retryCount = 0;
    while (!client.connect(THINGNAME) && retryCount < 5) {
        sendNotificationEmail("ESP32 Alert","Trying to reconnect to AWS IoT Core", retryCount);
        Serial.print(".");
        digitalWrite(ledPin, HIGH);  // Turn on the LED
        delay(1000);
        digitalWrite(ledPin, LOW);  // Turn on the LED
        delay(500);
        digitalWrite(ledPin, HIGH);  // Turn on the LED
        delay(1000);
        digitalWrite(ledPin, LOW);  // Turn on the LED
        delay(500);
        digitalWrite(ledPin, HIGH);  // Turn on the LED
        delay(1000);
        digitalWrite(ledPin, LOW);  // Turn on the LED
        delay(1000);
        retryCount++;
    }

    if (!client.connected()) {
        if (awsConnected) {  // Was connected before, now disconnected
            sendNotificationEmail("ESP32 Alert", "Lost AWS IOT Connection. Restarting...");
            awsConnected = false;
        }
        Serial.println("Failed to connect to AWS IoT Core. Restarting...");
        ESP.restart();
    } else {
        if (!awsConnected) {  // Just connected
            sendNotificationEmail("ESP32 Alert", "AWS IOT Connection Restored!");
            awsConnected = true;
        }
    }


    // Subscribe to a topic
    client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
    delay(1000);
    Serial.println("Connected to AWS IoT Core");
    digitalWrite(ledPin, HIGH);  // Turn on the LED
}


void connectAWS() {
    connectToWiFi();
    if (WiFi.status() == WL_CONNECTED) {
        connectToAWSIoT();
    }
}

void publishMessage()
{
  StaticJsonDocument<400> doc;
  doc["heat_index"] = hif;
  doc["humidity"] = h;
  doc["temperature"] = t;
  doc["datetime"] = currentDateTime;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}
 
void setup()
{
  delay(15000); //delay start to allow for syncronization timing with serial monitor
  Serial.begin(115200);
  
  // configure email to sms 
  smtp.debug(1);  // Enable debug output
  smtp.callback(smtpCallback);  // Set callback function

  Session_Config config;
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;

  connectAWS();
  timeClient.begin();
  dht.begin();
  delay(1000);
  lastLoopTime = millis();
}
 
void loop()
{

  h = dht.readHumidity();
  t = dht.readTemperature(true);
  hif = dht.computeHeatIndex(t, h);
 
  timeClient.update(); // Update NTP client to get current time

  // Get the current time
  currentDateTime = getFormattedDateTime();

  if (isnan(h) || isnan(t) )  // Check if any reads failed and exit early (to try again).
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  

  if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Disconnected from Wi-Fi. Attempting reconnection...");
      connectToWiFi();
      sendNotificationEmail("ESP32 Alert","Disconnected from Wifi. Attempting reconnection...");
  } else if (!client.connected()) {
      Serial.println("Disconnected from AWS IoT. Attempting reconnection...");
      connectToAWSIoT();
      sendNotificationEmail("ESP32 Alert","Disconnected from AWS IoT. Attempting reconnection...");
  }
  // Ensure we are still subscribed to the topic
  if (!client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC)) {
      Serial.println("Subscription to topic failed");
  }

  Serial.println(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°F "));
  Serial.print(F("°F  Heat index: "));
  Serial.print(hif);
  Serial.println(F("°F"));
  Serial.println(F("Date Time: "));
  Serial.print(currentDateTime);

  publishMessage();
  client.loop();
  delay(1000);

  if (millis() - lastLoopTime > 10000) { // 10 seconds as an example
        Serial.println("Loop frozen or taking too long. Restarting...");
        ESP.restart();
    }
    lastLoopTime = millis();
}