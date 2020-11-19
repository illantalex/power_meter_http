#include <ArduinoJson.h>
#include <HttpClient.h>
#include <PZEM004Tv30.h>
#include <SPI.h>
#include <UIPEthernet.h>

// Name of the server we want to connect to
const char server[] = "192.168.1.103";
// Mac address of the Ethernet
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

char buffer_http[32];
// Path to download (this is the bit after the hostname in the URL
// that you want to download
const char pathMeasure[] = "/measurement";
const char pathTime[] = "/service/time";
// Number of milliseconds to wait without receiving any data before we give up
// const int kNetworkTimeout = 30 * 1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;
uint32_t timeInUnixFormat = 0;

EthernetClient client;
HttpClient http(client);

uint32_t getRequest(const char path[]) {
  if (http.get(server, path)) Serial.println("request failed");

  Serial.print("Status code: ");
  Serial.println(http.responseStatusCode());

  http.skipResponseHeaders();

  uint32_t contentLen = http.contentLength();
  Serial.print("Body length: ");
  Serial.println(contentLen);

  for (uint32_t i = 0; i < contentLen && (http.available() || http.connected()); ++i) {
    if (http.available()) {
      buffer_http[i] = http.read();
    } else {
      Serial.println("No data available, please wait...");
      delay(kNetworkDelay);
    }
  }
  http.stop();
  return contentLen;
}

uint32_t getTime() {
  return timeInUnixFormat + millis() / 1000;
}

void setup() {
  // initialize serial communications at 9600 bps:

  Serial.begin(9600);
  Serial.println("start");

  while (Ethernet.begin(mac) != 1) {
    Serial.println("Error getting IP address via DHCP, trying again...");
    delay(15000);
  }
  Serial.print("localIP: ");
  Serial.println(Ethernet.localIP());
  Serial.print("subnetMask: ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("gatewayIP: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("dnsServerIP: ");
  Serial.println(Ethernet.dnsServerIP());

  int mill1 = millis();
  // int timeLen = 10;
  int timeLen = getRequest(pathTime);
  int mill2 = millis();

  // And just stop, now that we've tried a download
  for (int i = 0; i < timeLen; ++i) {
    timeInUnixFormat *= 10;
    timeInUnixFormat += buffer_http[i] - '0';
  }
  timeInUnixFormat -= (mill1 + mill2) / 2000;
  Serial.println();
  Serial.println(timeInUnixFormat);
  Serial.println(getTime());
  delay(10000);
  Serial.println(getTime());
}

void loop() {
}
