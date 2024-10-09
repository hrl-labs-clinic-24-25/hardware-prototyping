
// include the SPI library:
#include <SPI.h>

// set pins 10 & 9 as the slave selects
const int CS_0 = 10;
const int CS_1 = 9;


void setup() {
  // set the CS_0 as an output:
  pinMode (CS_0, OUTPUT); // Outputs pulled up in hardware
  pinMode (CS_1, OUTPUT);
  // initialize SPI:
  SPI.begin(); 
  // get serial monitor printing
  Serial.begin(9600);
  while (!Serial) delay(10);
  Serial.println("Serial connected.");
  // initialize both DACs
  int initDAC = 0b001000000000000000010010; //0010 0000 0000 0000 0001 0010 to init
  dacPackage(CS_0, initDAC);
    Serial.println("DAC_0 initialized.");
  dacPackage(CS_1, initDAC);
    Serial.println("DAC_1 initialized.");

  int packet0 = 0b000111111111111111111111; // Should be +5V out (assuming unsigned binary)
  dacPackage(CS_0, packet0);

  int packet1 = 0b000110000000000000000000; // Should be +2.5V out
  dacPackage(CS_1, packet1);
}

void loop() {
  delay(5000);
  int packet0 = 0b000110000000000000000000; // 2.5V
  dacPackage(CS_0, packet0);
  int packet1 = 0b000111111111111111111111; // 5V
  dacPackage(CS_1, packet1);

  delay(5000);

  int packet2 = 0b000111111111111111111111; // 5V
  dacPackage(CS_0, packet2);
  int packet3 = 0b000110000000000000000000; // 2.5V
  dacPackage(CS_1, packet3);

  // // delay(1000);
  // // Serial.println("I looped.");

}

void dacPackage(int chipSelect, int threeBytes) {
  // gain control of the SPI port and configure settings
  SPI.beginTransaction(SPISettings(3400000, MSBFIRST, SPI_MODE1));  // set feather m0 clock to 3.4MHz
  int upperByte = (threeBytes >> 16) & 0xFF; // Extract upper 8 bits
  int middleByte = (threeBytes >> 8) & 0xFF;
  int bottomByte = threeBytes & 0xFF;
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
