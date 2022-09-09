#include <WiFi.h>
#include <DNSServer.h>

WiFiServer server(80);
DNSServer dnsServer;
IPAddress apIP(192, 168, 4, 1);

const char *ssid = "Bike";
const char *password = "";
bool inFindMode = false;

void setup()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  dnsServer.start(53, "*", apIP);
  server.begin();
}

const int blinkLoops = 400;
unsigned int prevBlink = 0;
bool pulseTurnedOn = false;

unsigned int connectTime = millis();
void loop()
{
  dnsServer.processNextRequest();

  WiFiClient client = server.available();   // listen for incoming clients
  Serial.println("-");
  if (client && client.connected()) {
    Serial.println("conn");
    inFindMode = true;
    connectTime = millis();
    client.stop();
  }
  if (millis() - connectTime > 1000 * 10)
  {
    inFindMode = false;
    digitalWrite(LED_BUILTIN, LOW);
  }

  if (inFindMode)
  {
    if (millis() - prevBlink > blinkLoops)
    {
      prevBlink = millis();
      pulseTurnedOn = !pulseTurnedOn;
      if (pulseTurnedOn)
      {
        digitalWrite(LED_BUILTIN, HIGH);
      } else digitalWrite(LED_BUILTIN, LOW);
    }
  }

}
