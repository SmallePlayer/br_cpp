#include <WiFi.h>
#include <WiFiClient.h>

const char* WIFI_SSID = "Keenetic-8281";
const char* WIFI_PASSWORD = "UYy5BHPc";
const char* SERVER_IP = "192.168.1.78";
const int SERVER_PORT = 8080;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  int a = 1;
  if (client.connect(SERVER_IP, SERVER_PORT)) {
    client.write((uint8_t*)&a, sizeof(a));
  }
  
}

int a = 1;
void loop() {
  if (!client.connected()) {
    client.stop();
    client.connect(SERVER_IP, SERVER_PORT);
    delay(1000);
  }
  
  if (client.connected()) {
    client.write((uint8_t*)&a, sizeof(a));
    delay(500);
  }
  a++;
}