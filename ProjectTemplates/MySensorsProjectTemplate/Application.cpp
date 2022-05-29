// The Arduino core library version
#define ARDUINO 10800

// The simulated board type
#define __AVR_ATmega328P__ // Arduino Nano

#include <VirtualBoardIncludes.h>

// For Visual C++ (MSVC) we need following functions at least as empty stubs
// Hint: For actual usage implement function in sketch.h file and comment out here
inline void preHwInit(void) {}
void before(void) {}
void receiveTime(uint32_t value) {}

// Include additional Arduino libraries *.cpp files here
//#include "../../../libraries/RfControl/src/RfControl.cpp"

#include "$projectname$/Sketch.h"
