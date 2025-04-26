
// include the SPI library:
#include <SPI.h>

// set pin 10 as the slave select
const int CS_0 = 10;

void setup() {
  // set the CS_0 as an output:
  pinMode (CS_0, OUTPUT); 
  digitalWrite (CS_0, HIGH);
  // initialize SPI:
  SPI.begin(); 
  // get serial monitor printing
  Serial.begin(9600);
  while (!Serial) delay(10);
  Serial.println("Serial connected.");
  // initialize DAC
  // dacPackage(CS_0, 0b00100000, 0b00000000, 0b00010010);
  //   Serial.println("DAC initialized.");

  // initialize DAC:
  int top = 0b00100000; 
  int mid = 0b00000000;
  int bot = 0b00010010;
  // send 5V to DACs
  int top1 = 0b00011000;
  int mid1 = 0b00000000;
  int bot1 = 0b00000000;
  dacPackage(CS_0, top, mid, bot); // init
  dacPackage(CS_0, top1, mid1, bot1); // package
}

void loop() {
  dacPackage(CS_0, top1, mid1, bot1); // package
  // delay(1000);
  // Serial.println("I looped.");
}

void dacPackage(int chipSelect, int upperByte, int middleByte, int bottomByte) {
  // gain control of the SPI port and configure settings
  SPI.beginTransaction(SPISettings(3400000, MSBFIRST, SPI_MODE1));  // currently 3.4MHz
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
