
// include the SPI library:
#include <SPI.h>

// set pin 10 as the slave select
const int CS = 10;
const int muxS0Pin = 7;

const int CS_0 = 0;
const int CS_1 = 1;

const int topRail = 0b000111111111111111111111;  // 5 V
const int botRail = 0b000110000000000000000000;  // 2.5 V

void setup() {
  // set the MCU CS pin as an output, and make sure it's high
  pinMode (CS, OUTPUT); 
  digitalWrite (CS, HIGH);

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
  dacPackage(CS_1,initDAC);
  Serial.println("DACs Initialized");
}

void loop() {
  
  dacPackage(CS_0, topRail); Serial.println("I SPI'ed CS_0 topRail");
  delay(1000);

  dacPackage(CS_1, topRail); Serial.println("I SPI'ed CS_1 topRail");
  delay(1000);
  
  dacPackage(CS_0, botRail); Serial.println("I SPI'ed CS_0 botRail");
  delay(1000);

  dacPackage(CS_1, botRail); Serial.println("I SPI'ed CS_1 botRail");
  delay(1000);
}

void dacPackage(int chipSelect, int threeBytes) {
  switch (chipSelect) {
    case CS_0:          // CS_0 corresponds to S2,S1,S0 = 000
      digitalWrite (muxS0Pin, LOW);
      break;
    case CS_1:
      digitalWrite (muxS0Pin, HIGH);
      break;
  }
  delay(1);

  // gain control of the SPI port and configure settings
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1));  // currently 4MHz
  int upperByte = (threeBytes >> 16) & 0xFF;  // Extract the upper 8 bits
  int middleByte = (threeBytes >> 8) & 0xFF;  // Extract the middle 8 bits
  int bottomByte = threeBytes & 0xFF;          // Extract the lower 8 bits
  // take the desired CS pin low to select the DAC:
  digitalWrite(CS,LOW);
  // send the 24 bit message over in 3 byte chunks:
  SPI.transfer(upperByte);
  SPI.transfer(middleByte);
  SPI.transfer(bottomByte);
  // take the desired CS pin high to de-select the chip:
  digitalWrite(CS,HIGH);
  // release control of the SPI port
  SPI.endTransaction();
}
