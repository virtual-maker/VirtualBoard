
// Enable debug prints to serial monitor
#define MY_DEBUG
// Enable support for I_DEBUG messages.
#define MY_SPECIAL_DEBUG
// Use a bit lower baudrate for serial prints on ESP8266 than default in MyConfig.h
#define MY_BAUD_RATE 74880

// Enable OTA debugging to Node 0
//#define MY_DEBUG_OTA (0)
// Allow sending logs without MY_DEBUG_OTA enabled
//#define MY_OTA_LOG_SENDER_FEATURE
// Disable echoing of debug messages
//#define MY_DEBUG_OTA_DISABLE_ECHO

// Enable signing related debug prints to serial monitor
//#define MY_DEBUG_VERBOSE_SIGNING 
// Select soft/hardware signing method
//#define MY_SIGNING_SOFT
// Enable this if you want destination node to sign all messages sent to this node.
//#define MY_SIGNING_REQUEST_SIGNATURES //!< destination node signs all messages sent to this node

// Static ID
#define MY_NODE_ID 210
// Force to connect to gateway with node ID 0
#define MY_PARENT_NODE_ID 0
// Disable fall back if parent node fails
#define MY_PARENT_NODE_IS_STATIC
// Disable the node to node transport attempts
#define MY_TRANSPORT_N2N_FEATURE_DISABLED

// Enable and select radio type attached
#define MY_ETHER_TCP
#define MY_ETHER_TCP_SERVER "localhost"
//#define MY_ETHER_TCP_SERVER "192.168.16.64"

// Check transport in regular intervals to detect HW issues and re-initialize in case of failure
#define MY_TRANSPORT_SANITY_CHECK
// Interval (in ms) for transport sanity checks
#define MY_TRANSPORT_SANITY_CHECK_INTERVAL_MS (1*60*1000ul)

#include <MySensors.h>

void presentation()
{
    sendSketchInfo("MySensors Node", "1.0");
}

void receive(const MyMessage& message)
{
}

void setup()
{
}

void loop()
{
}
