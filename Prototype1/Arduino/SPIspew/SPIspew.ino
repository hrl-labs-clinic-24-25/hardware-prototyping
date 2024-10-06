
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
}

void loop() {

    int top = 0b00010111; //0010 0000 0000 0000 0001 0010
    int mid = 0b11111111;
    int bot = 0b11111111;
    dacPackage(CS_0, top, mid, bot);
    // delay(1000);
    // Serial.println("I looped.");

}

void dacPackage(int chipSelect, int upperByte, int centerByte, int bottomByte) {
  // gain control of the SPI port and configure settings
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1));  // currently 4MHz
  // take the desired CS pin low to select the DAC:
  digitalWrite(chipSelect,LOW);
  // send the 24 bit message over in 3 byte chunks:
  SPI.transfer(upperByte);
  SPI.transfer(centerByte);
  SPI.transfer(bottomByte);
  // take the desired CS pin high to de-select the chip:
  digitalWrite(chipSelect,HIGH);
  // release control of the SPI port
  SPI.endTransaction();
}
