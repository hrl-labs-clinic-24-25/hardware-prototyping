
// include the SPI library:
#include <SPI.h>

// set pin 10 as the slave select
const int CS_0 = 10;
const int buttonHighPin = 21;
const int buttonLowPin = 20;

const int muxS0Pin = 7;

void setup() {
  // set the CS_0 as an output, buttons as inputs:
  pinMode (CS_0, OUTPUT); 
  digitalWrite (CS_0, HIGH);

  pinMode (buttonHighPin, INPUT);
  pinMode (buttonLowPin, INPUT);

  pinMode (muxS0Pin, OUTPUT);
  digitalWrite (muxS0Pin, LOW);

  // initialize SPI:
  SPI.begin(); 
  // get serial monitor printing
  Serial.begin(9600);
  while (!Serial) delay(10);
  Serial.println("Serial connected.");
  int initDAC = 0b001000000000000000010010;
  dacPackage(CS_0,initDAC);
  Serial.println("DAC Initialized");
}

void loop() {
  int buttonHigh = 0;
  int buttonLow = 0;
  buttonHigh = digitalRead(buttonHighPin);
  buttonLow = digitalRead(buttonLowPin);

  if (buttonHigh) {
    int topRail = 0b000101111111111111111111;
    dacPackage(CS_0, topRail); Serial.println("I SPI'ed CS_0 High");
    delay(50);
    while (digitalRead(buttonHighPin)) {dacPackage(CS_0, topRail); delayMicroseconds(6);}
    delay(50);
    Serial.println("I finished SPI CS_0 High");
  }
  if (buttonLow) {
    int botRail = 0b000110000000000000000000;
    dacPackage(CS_0, botRail); Serial.println("I SPI'ed CS_0 Low");
    delay(50);
    while (digitalRead(buttonLowPin)) {dacPackage(CS_0, botRail); delayMicroseconds(6);}
    delay(50);
    Serial.println("I finished SPI CS_0 Low");
  }
}

void dacPackage(int chipSelect, int threeBytes) {
  // gain control of the SPI port and configure settings
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1));  // currently 4MHz
  int upperByte = (threeBytes >> 16) & 0xFF;  // Extract the upper 8 bits
  int middleByte = (threeBytes >> 8) & 0xFF;  // Extract the middle 8 bits
  int bottomByte = threeBytes & 0xFF;          // Extract the lower 8 bits
  // take the desired CS pin low to select the DAC:
  digitalWrite(chipSelect,LOW);
  // send the 24 bit message over in 3 byte chunks:
  SPI.transfer(upperByte);
  SPI.transfer(middleByte);
  SPI.transfer(bottomByte);
  // take the desired CS pin high to de-select the chip:
  digitalWrite(chipSelect,HIGH);
  // release control of the SPI port
  SPI.endTransaction();
}
