#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266TelegramBOT.h>

/*-------------------------------------------------------------------
*******************************************************************
-------------------------------------------------------------------*/

// telegram setup 
#define BOT_TOKEN "-------------------------" // Insert your BotToken 
#define CHAT_ID "------------------" // Insert your ChatID 

WiFiClientSecure secured_client;
TelegramBOT bot(BOT_TOKEN, "ESP_IoTBot", "ESP_IoTBot");

const char* ssid = "----------------------"; // Enter your WLAN name 
const char* password = "-----------------------------"; // Enter your WLAN password

// PushBullet configuration
const char* host = "api.pushbullet.com";
const int httpsPort = 443;
const char* PushBulletAPIKEY = {""}; // Enter Pushbullet API KEY 
const char* fingerprint = "51:93:C1:74:C7:B6:B4:10:04:CB:E1:F1:AE:22:FB:26:07:50:B1:34";

// PushingBox scenario DeviceId code and API
String deviceId = "v9EC62998E29378B";
const char* logServer = "api.pushingbox.com";

/*-------------------------------------------------------------------
*******************************************************************
-------------------------------------------------------------------*/

void connect2Wifi() {
  Serial.print("Connecting to "); 
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nWifi connected");
}

void sendTelegram(String message, String chatID) {
  Serial.println("Sending Telegram message...");
  bot.sendMessage(chatID, message, "");
}

/*------------------------------------------------------------------
*******************************************************************
-------------------------------------------------------------------*/

void sendPushbullet(String message){

  Serial.println("- connecting to Home Router SSID: " + String(ssid));
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("- succesfully connected");
  Serial.println("- starting client");
  
  WiFiClient client;

  Serial.println("- connecting to pushing server: " + String(logServer));
  if (client.connect(logServer, 80)) {
    Serial.println("- succesfully connected");
    
    String postStr = "devid=";
    postStr += String(deviceId);
    postStr += "&message_parameter=";
    postStr += String(message);
    postStr += "\r\n\r\n";
    
    Serial.println("- sending data...");
    
    client.print("POST /pushingbox HTTP/1.1\n");
    client.print("Host: api.pushingbox.com\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
  }
  client.stop();
  Serial.println("- stopping the client");
}

/*------------------------------------------------------------------
*******************************************************************
-------------------------------------------------------------------*/

void setup() {
  Serial.begin(115200);
  connect2Wifi();

  sendTelegram("Es klingelt!", CHAT_ID);
  sendPushbullet("Es klingelt!");

  ESP.deepSleep(0);
}

void loop() {}
