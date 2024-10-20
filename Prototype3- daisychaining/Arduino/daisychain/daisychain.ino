
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
  // digitalWrite(CS_0,LOW);
  int top = 0b00100000; 
  int mid = 0b00000000;
  int bot = 0b00010010;
  // digitalWrite(CS_0,HIGH);

  // send 5V to DACs
  // digitalWrite(CS_0,LOW);
  int top1 = 0b00011000;
  int mid1 = 0b00000000;
  int bot1 = 0b00000000;
  // digitalWrite(CS_0,HIGH);

  dacPackage(CS_0, top, mid, bot); // init
  dacPackage(CS_0, top1, mid1, bot1); // 5V package

  // delay(500);

}

void loop() {
  // byte dummySDOByte = SPI.transfer(0x00); // Receive data by sending a dummy byte
  // byte secondDummyByte = SPI.transfer(0b00111111);
  // Serial.print("Dummy SDO Byte: ");
  // Serial.println(dummySDOByte);
  // Serial.print("Second dummy byte:");
  // Serial.println(secondDummyByte);


    // delay(1000);
    // Serial.println("I looped.");
    // delay(1000);

  // int top1 = 0b00011010;
  // int mid1 = 0b00000000;
  // int bot1 = 0b00000000;
  // dacPackage(CS_0, top1, mid1, bot1); // package

  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE1));  // currently 3.4MHz
  
  // take the desired CS pin low to select the DAC:
  digitalWrite(CS_0,LOW);
  // send the 24 bit message over in 3 byte chunks:
  byte oneByte = 0b00011010; //0b00000001
  byte twoByte = 0b00000000; //0b00000010
  byte threeByte = 0b00000000; //0b00000011
  byte fourByte = 0b00011010; //0b00000101
  byte fiveByte = 0b00000000; //0b00000111
  byte sixByte = 0b00000000; //0b00001000
  
  SPI.transfer(oneByte);
  SPI.transfer(twoByte);
  SPI.transfer(threeByte);
  // delayMicroseconds(10);
  // SPI.transfer(fourByte);
  // SPI.transfer(fiveByte);
  // SPI.transfer(twoByte);

  // Serial.print("upper: ");
  // Serial.println(oneByte);
  // Serial.print("center: ");
  // Serial.println(twoByte);
  // Serial.print("bottom: ");
  // Serial.println(threeByte);
  // Serial.print("bottomest: ");
  // Serial.println(fourByte);

  // take the desired CS pin high to de-select the chip:
  digitalWrite(CS_0,HIGH);
  // release control of the SPI port
  SPI.endTransaction();

  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE1));
  digitalWrite(CS_0, LOW);

  SPI.transfer(fourByte);
  SPI.transfer(fiveByte);
  SPI.transfer(sixByte);
  digitalWrite(CS_0, HIGH);
  SPI.endTransaction();

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
  // bottomestByte = SPI.transfer(bottomestByte);
  // Serial.print("upper: ");
  // Serial.println(upperByte);
  // Serial.print("center: ");
  // Serial.println(centerByte);
  // Serial.print("bottom: ");
  // Serial.println(bottomByte);

  // take the desired CS pin high to de-select the chip:
  digitalWrite(chipSelect,HIGH);
  // release control of the SPI port
  SPI.endTransaction();
}
