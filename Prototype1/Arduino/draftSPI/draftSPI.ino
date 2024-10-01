
// include the SPI library:
#include <SPI.h>


// set pin 10 as the slave select for the digital pot:
const int CS_0 = 10;
const int buttonHigh = 17;
const int buttonLow = 7;

void setup() {
  // Establish a button to send data back and forth
  pinMode (buttonHigh, INPUT);
  pinMode (buttonLow, INPUT);

  // set the CS_0 as an output:
  pinMode (CS_0, OUTPUT);
  digitalWrite (CS_0, HIGH);
  // initialize SPI:
  SPI.begin(); 
}

void loop() {

  if (buttonHigh) {
    int top = 0b11101110;
    int mid = 0b11011111;
    int bot = 0b10110101;
    dacPackage(currentChip, top, mid, bot);
    delay(50);
    while (buttonHigh) {}
    delay(50);
  }

  if (buttonLow) {
    int top = 0b10000001;
    int mid = 0b10010101;
    int bot = 0b10100011;
    dacPackage(currentChip, top, mid, bot);
    delay(50);
    while (buttonLow) {}
    delay(50);
  }

}

void dacPackage(int chipSelect, int upperByte, int centerByte, int bottomByte) {
  // gain control of the SPI port and configure settings
  SPI.beginTransaction(SPISettings(100, MSBFIRST, SPI_MODE1));
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
