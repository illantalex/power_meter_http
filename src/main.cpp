// #include <Arduino.h>
// #include <PubSubClient.h>
// #include <SPI.h>
// #include <UIPEthernet.h>
// #include <Dns.h>

// EthernetClient ethClient;
// PubSubClient client(ethClient);
// signed long next;
// EthernetUDP udp;
// IPAddress server(192, 168, 1, 103);

// // char server[] = "illantal-win";
// uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};

// // void reconnect();

// void get_network_and_broadcast(IPAddress ip, IPAddress mask, IPAddress &network, IPAddress &broadcast) {
//     for (int i = 0; i < 4; ++i) {
//         network[i] = ip[i] & mask[i];
//         broadcast[i] = network[i] + ~mask[i];
//     }
// }

// // IPAddress find_server(IPAddress network, IPAddress broadcast) {
// //   uint32_t network_int = uint32_t(network);
// //   uint32_t broadcast_int = uint32_t(broadcast);
// //   for (uint32_t ip = network_int + 2; ip < broadcast_int; ++ip) {
// //     // client.setServer(IPAddress(ip), 1883);
// //     // if (client.connect("ArduinoClient")) {
// //     //   return IPAddress(ip);
// //     // }
// //     // Serial.println("fail");
// //   }
// //   return IPAddress(0, 0, 0, 0);
// // }

// // void callback(char* topic, byte* payload, unsigned int length) {
// //     Serial.print("Message arrived [");
// //     Serial.print(topic);
// //     Serial.print("] ");
// //     for (int i = 0; i < length; i++) {
// //         Serial.print((char)payload[i]);
// //     }
// //     Serial.println();
// // }

// // void reconnect() {
// //     // Loop until we're reconnected
// //     while (!client.connected()) {
// //         Serial.print("Attempting MQTT connection...");
// //         // Attempt to connect
// //         if (client.connect("arduinoClient")) {
// //             Serial.println("connected");
// //             // Once connected, publish an announcement...
// //             client.publish("test", "hello world");
// //             // ... and resubscribe
// //             client.subscribe("inTopic");
// //         } else {
// //             Serial.print("failed, rc=");
// //             Serial.print(client.state());
// //             Serial.println(" try again in 5 seconds");
// //             // Wait 5 seconds before retrying
// //             delay(1000);
// //         }
// //     }
// // }

// void setup() {
//     Serial.begin(9600);

//     if (!Ethernet.begin(mac)) {
//         Serial.println("Ethernet failed.");
//         return;
//     } else {
//         Serial.print("localIP: ");
//         Serial.println(Ethernet.localIP());
//         Serial.print("subnetMask: ");
//         Serial.println(Ethernet.subnetMask());
//         Serial.print("gatewayIP: ");
//         Serial.println(Ethernet.gatewayIP());
//         Serial.print("dnsServerIP: ");
//         Serial.println(Ethernet.dnsServerIP());
//     }

//     // client.setServer(broadcast(Ethernet.localIP(), Ethernet.subnetMask()), 1883);
//     //   client.setServer(server, 1883);

//     //   client.setCallback(callback);
//     IPAddress network, broadcast;
//     get_network_and_broadcast(Ethernet.localIP(), Ethernet.subnetMask(), network, broadcast);
//     // find_server(network, broadcast);
//     delay(1500);
// }

// void loop() {
//     //   if (!client.connected()) {
//     //     reconnect();
//     //   }

//     //   client.loop();
//     IPAddress network, broadcast;
//     get_network_and_broadcast(Ethernet.localIP(), Ethernet.subnetMask(), network, broadcast);
//     int success = udp.beginPacket(network, 5005);
//     Serial.print("beginPacket: ");
//     Serial.println(success ? "success" : "failed");
//     success = udp.write("hello world from arduino");

//     Serial.print("bytes written: ");
//     Serial.println(success);

//     success = udp.endPacket();

//     Serial.print("endPacket: ");
//     Serial.println(success ? "success" : "failed");
//     delay(3000);
// }

// (c) Copyright 2010-2012 MCQN Ltd.
// Released under Apache License, version 2.0
//
// Simple example to show how to use the HttpClient library
// Get's the web page given at http://<kHostname><kPath> and
// outputs the content to the serial port

#include <UIPEthernet.h>
// #include <EthernetClient.h>
#include <HttpClient.h>
#include <SPI.h>

// This example downloads the URL "http://arduino.cc/"

// Name of the server we want to connect to
const char kHostname[] = "192.168.1.103";
// Path to download (this is the bit after the hostname in the URL
// that you want to download
const char kPath[] = "/";

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30 * 1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  while (Ethernet.begin(mac) != 1) {
    Serial.println("Error getting IP address via DHCP, trying again...");
    delay(15000);
  }
}

void loop() {
  int err = 0;

  EthernetClient c;
  HttpClient http(c);

  err = http.get(kHostname, kPath);
  if (err == 0) {
    Serial.println("startedRequest ok");

    err = http.responseStatusCode();
    if (err >= 0) {
      Serial.print("Got status code: ");
      Serial.println(err);

      // Usually you'd check that the response code is 200 or a
      // similar "success" code (200-299) before carrying on,
      // but we'll print out whatever response we get

      err = http.skipResponseHeaders();
      if (err >= 0) {
        int bodyLen = http.contentLength();
        Serial.print("Content length is: ");
        Serial.println(bodyLen);
        Serial.println();
        Serial.println("Body returned follows:");

        // Now we've got to the body, so we can print it out
        unsigned long timeoutStart = millis();
        char c;
        // Whilst we haven't timed out & haven't reached the end of the body
        while ((http.connected() || http.available()) &&
               ((millis() - timeoutStart) < kNetworkTimeout)) {
          if (http.available()) {
            c = http.read();
            // Print out this character
            Serial.print(c);

            bodyLen--;
            // We read something, reset the timeout counter
            timeoutStart = millis();
          } else {
            // We haven't got any data, so let's pause to allow some to
            // arrive
            delay(kNetworkDelay);
          }
        }
      } else {
        Serial.print("Failed to skip response headers: ");
        Serial.println(err);
      }
    } else {
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  } else {
    Serial.print("Connect failed: ");
    Serial.println(err);
  }
  http.stop();

  // And just stop, now that we've tried a download
  while (1)
    ;
}
