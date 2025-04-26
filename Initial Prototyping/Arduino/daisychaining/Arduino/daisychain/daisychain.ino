
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

  // initialize DAC:
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE1));
  digitalWrite(CS_0, LOW);
  int top = 0b00100000; 
  int mid = 0b00000000;
  int bot = 0b00010010;
  SPI.transfer(top);
  SPI.transfer(mid);
  SPI.transfer(bot);
  SPI.transfer(top);
  SPI.transfer(mid);
  SPI.transfer(bot);
  digitalWrite(CS_0, HIGH);
  SPI.endTransaction();
  Serial.println("DAC initialized.");

  // // send 5V to DACs
  // SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE1));
  // digitalWrite(CS_0, LOW);
  // int top1 = 0b00011000;
  // int mid1 = 0b00000000;
  // int bot1 = 0b00000000;
  // SPI.transfer(top1);
  // SPI.transfer(mid1);
  // SPI.transfer(bot1);
  // SPI.transfer(top1);
  // SPI.transfer(mid1);
  // SPI.transfer(bot1);
  // digitalWrite(CS_0, HIGH);
  // SPI.endTransaction();
  // Serial.println("Sent 5V.");

  // delay(2000);

}

void loop() {
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE1));  // currently 3.4MHz
  digitalWrite(CS_0,LOW);
  // send the 48 bit message over in 6 byte chunks:
  byte oneByte = 0b00011000; //0b00000001
  byte twoByte = 0b00000000; //0b00000010
  byte threeByte = 0b00000000; //0b00000011
  byte fourByte = 0b00011010; //0b00000101
  byte fiveByte = 0b00000000; //0b00000111
  byte sixByte = 0b00000000; //0b00001000
  
  SPI.transfer(oneByte);
  SPI.transfer(twoByte);
  SPI.transfer(threeByte);
  // Serial.println("Sent first three bytes");
  // delay(2000);
  // delayMicroseconds(50);
  SPI.transfer(fourByte);
  SPI.transfer(fiveByte);
  SPI.transfer(twoByte);
  // Serial.println("Sent second three bytes");
  // delay(2000);

  digitalWrite(CS_0,HIGH);
  // Serial.println("CS HIGH");
  // release control of the SPI port
  SPI.endTransaction();

  // SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE1));
  // digitalWrite(CS_0, LOW);

  // SPI.transfer(fourByte);
  // SPI.transfer(fiveByte);
  // SPI.transfer(sixByte);
  // digitalWrite(CS_0, HIGH);
  // SPI.endTransaction();

}

void dacPackage(int chipSelect, int upperByte, int centerByte, int bottomByte) {
  // gain control of the SPI port and configure settings
  SPI.beginTransaction(SPISettings(3400000, MSBFIRST, SPI_MODE1));  // currently 3.4MHz
  // take the desired CS pin low to select the DAC:
  digitalWrite(chipSelect,LOW);
  // send the 24 bit message over in 3 byte chunks:
  upperByte = SPI.transfer(upperByte);
  centerByte = SPI.transfer(centerByte);
  bottomByte = SPI.transfer(bottomByte);

  // take the desired CS pin high to de-select the chip:
  digitalWrite(chipSelect,HIGH);
  // release control of the SPI port
  SPI.endTransaction();
}
