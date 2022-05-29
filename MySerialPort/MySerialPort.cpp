// This is a development and test project for new VirtualHardwareWrapper features

// Defines for Virtual-Maker hardware access

// Use IO-Warrior hardware
#define VM_USE_HARDWARE

// Connect IO-Warrior with unique serial number (useful for multiple connected devices)
// #define VM_HW_SERIAL_NUMBER "100041F0"

// Use virtual interface for TWI/I2C (VirtualTwiServer must be running)
// #define VM_USE_VIRTUAL_TWI

// ToDo: #iwa Use virtual interface for SPI (ToDo: #iwa VirtualSpiServer must be running)
// #define VM_USE_VIRTUAL_SPI

// Call _yield in delay() function (executed delay time will be less accurate)
// #define VM_CALL_YIELD_WHILE_DELAY

// Use serial port from this machine for HWSERIAL1
#define VM_HWSERIAL1_PORTNAME "COM1"

// End of VM_ defines

#define ARDUINO 10800
#define __AVR_ATmega328P__ // Arduino UNO

#include <VirtualBoard.h>

#include <Wire.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <SoftwareSerial.h>

#define PCA9555 B01000000/2
#define PCA9555_INPUT_0 0
#define PCA9555_INPUT_1 1
#define PCA9555_OUTPUT_0 2
#define PCA9555_OUTPUT_1 3
#define PCA9555_PI_0 4
#define PCA9555_PI_1 5
#define PCA9555_CFG_0 6
#define PCA9555_CFG_1 7

// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 13;

void spiTransferTwoBytes(int value1, int value2)
{
  SPI.beginTransaction(SPISettings());

  // take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin, LOW);
  delay(100);
  //  send in the address and value via SPI:
  SPI.transfer(value1);
  SPI.transfer(value2);
  delay(100);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin, HIGH);

  SPI.endTransaction();
}

void setDacOutput(int channel, int value)
{
  if (value > 4095 || value < 0) {
    return;
  }

  // Thanks to: www.kerrywong.com/2012/07/25/code-for-mcp4821-mcp4822/

  byte lowByte = (byte)(value & 0xFF);
  byte highByte = (byte)(((value >> 8) & 0xFF) | channel << 7 | 1 << 5 | 1 << 4);

  spiTransferTwoBytes(highByte, lowByte);
}

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// the DNS server IP
IPAddress dnServer(192, 168, 0, 1);
// the router's gateway address:
IPAddress gateway(192, 168, 0, 1);
// the subnet:
IPAddress subnet(255, 255, 255, 0);

//the IP address is dependent on your network
IPAddress ip(192, 168, 0, 2);


unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged";        // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

IPAddress broadcast(192, 168, 16, 255);
//IPAddress broadcast(192, 168, 16, 117);

void sendUdpBroadcast()
{
    // start UDP
    Udp.begin(localPort);

    for (size_t i = 0; i < 100; i++)
    {
        Udp.stop();
        Udp.begin(localPort);
    }

    Udp.beginPacket(broadcast, 63256);
    Udp.write(ReplyBuffer);
    Udp.endPacket();
}

void ethernetSetup()
{
    Serial.begin(9600);

    // initialize the Ethernet device
    Ethernet.begin(mac, ip, dnServer, gateway, subnet);

    //print out the IP address
    IPAddress ip = Ethernet.localIP();
    IPAddress dns = Ethernet.dnsServerIP();
    IPAddress gateway = Ethernet.gatewayIP();
    IPAddress subnet = Ethernet.subnetMask();

    Serial.print("IP = ");
    Serial.println(ip);
    Serial.print("DNS = ");
    Serial.println(dns);
    Serial.print("GATEWAY = ");
    Serial.println(gateway);
    Serial.print("SUBNET = ");
    Serial.println(subnet);

    sendUdpBroadcast();
}

void setup()
{
    ethernetSetup();

    // set the slaveSelectPin as an output:
    pinMode(slaveSelectPin, OUTPUT);
    // initialize SPI:
    SPI.begin();
    // write value to SPI device
    setDacOutput(0, 1000);
    //return;

    pinMode(LED_BUILTIN, OUTPUT);

    //Serial1.begin(9600);
    //Serial1.println("Hello, World!");
    //Serial1.flush();

    Wire.begin();

    //Both Ports as Output
    Wire.beginTransmission(PCA9555);
    Wire.write(PCA9555_CFG_0);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission();

}

void loop()
{
  //delay(1000);
  //return;

  uint32_t millis = 25;

  Wire.beginTransmission(PCA9555);
  Wire.write(PCA9555_OUTPUT_0);
  Wire.write(0xff);
  Wire.write(0xff);
  byte result = Wire.endTransmission();
  delay(millis);

  Wire.beginTransmission(PCA9555);
  Wire.write(PCA9555_OUTPUT_0);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(millis);

  return;

  //if (Serial1.available()) {
  //  uint8_t c = Serial1.read();
  //  Serial.write(c);
  //  Serial1.write(c);
  //}
  //return;

  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
