#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "******";
const char* password = "*******";

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port
char incomingPacket[255];  // the buffer for the incoming packets
char  replyPacekt[] = " Message received";  // reply from the ESP


void setup()
{
  pinMode(D1, OUTPUT); // Setup the pin
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}


void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    if (strcmp(incomingPacket, "On") == 0)  //Pin turning On
    {
      Udp.write("On!");
      Serial.printf("On!");
      digitalWrite(D1, HIGH);
      delay(1000);
    }
    else if(strcmp(incomingPacket, "Off")== 0) //Pin off
    {
      Udp.write("Off!");
      Serial.printf("Off!");
      digitalWrite(D1, LOW);
    }
    else    //Asking the sender what is goin' on
    {
     Serial.printf("On or Off?");
     Udp.write("On or off?");
    }

    // send back a reply, to the IP address and port we got the packet from
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacekt);
    Udp.endPacket();
  }
}
