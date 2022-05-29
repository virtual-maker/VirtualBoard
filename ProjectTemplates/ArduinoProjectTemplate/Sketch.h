void setup()
{
	Serial.begin(115200);
	Serial.println("Hello, World!");
}

// Pay attention that int is int32_t with MSVC
// This differs from e.g. Arduino Uno where int is int16_t
int c = 0; 

void loop()
{
	Serial.println(c);
	c++;
	delay(500);
}
