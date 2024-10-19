
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
<<<<<<< HEAD:Prototype1- SPI connected/Arduino/SPIspew/SPIspew.ino
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
    // delay(1000);
    // Serial.println("I looped.");
=======
  int initDAC = 0b001000000000000000000010;
  dacPackage(CS_0,initDAC);
  Serial.println("DAC Initialized");
}

void loop() {

    //int value = 0b000101111111111111111111;     // topRail Value
    int value = 0b000110001111111111111111;     // middle Value
    //int value = 0b000111111000000000000000;     // botRail Value
    //int value = 0b000110000000000000000000;     // botRail Value

    dacPackage(CS_0, value);
    // delay(1000);
    // Serial.println("I looped.");

    delayMicroseconds(6);
>>>>>>> c149827 (Jackson's P1 Mux Changes):Prototype1/Arduino/SPIspew/SPIspew.ino
}

void dacPackage(int chipSelect, int threeBytes) {
  // gain control of the SPI port and configure settings
<<<<<<< HEAD:Prototype1- SPI connected/Arduino/SPIspew/SPIspew.ino
  SPI.beginTransaction(SPISettings(3400000, MSBFIRST, SPI_MODE1));  // currently 3.4MHz
=======
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1));  // currently 4MHz
  int upperByte = (threeBytes >> 16) & 0xFF;  // Extract the upper 8 bits
  int middleByte = (threeBytes >> 8) & 0xFF;  // Extract the middle 8 bits
  int bottomByte = threeBytes & 0xFF;          // Extract the lower 8 bits
>>>>>>> c149827 (Jackson's P1 Mux Changes):Prototype1/Arduino/SPIspew/SPIspew.ino
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
