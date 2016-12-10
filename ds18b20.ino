#include <OneWire.h>

/* DS18S20 Temperature chip i/o */

OneWire  ds(9);  // on pin 9
uint8_t addr[8];

void setup(void) 
{
  Serial.begin(115200);
  if(!ds.search(addr)) {
    Serial.print("No device found !");
    ds.reset_search();
    delay(250);
    return;
  }
  Serial.print("Reading:");
  for(uint8_t i=0; i<8; i++) {
    Serial.print(" ");
    Serial.print(addr[i], HEX);
  }
  Serial.println("");
  if(OneWire::crc8( addr, 7) == addr[7]) {
    Serial.print("GOOD: CRC is valid ! : 0x"); Serial.println(addr[7], HEX);
  }
  else {
    Serial.println("BAD: CRC is NOT valid!");
    Serial.print("  Expected: 0x"); Serial.print(OneWire::crc8( addr, 7), HEX); Serial.println();
    Serial.print("  Got: 0x"); Serial.print(addr[7], HEX); Serial.println();
    return;
  }
  if(0x28 != addr[0]) {
      Serial.print("Device is not a DS18S20 family device.\n");
      Serial.print("  Expected: 0x"); Serial.print("28"); Serial.println();
      Serial.print("  Got: 0x"); Serial.print(addr[0], HEX); Serial.println();
      return;
  }
}

void loop(void) 
{
  Serial.println("============= LOOP ==============");

  ds.reset();
  ds.select(addr);
  Serial.println("Starting conversion");
  ds.write(0x44,1);         // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  uint8_t present = ds.reset();
  ds.select(addr);
  Serial.println("Writing 0xBE");
  ds.write(0xBE);         // Read Scratchpad

  Serial.print("Reading:");
  uint8_t data[8] = {0};
  for (uint8_t i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    Serial.print(" ");
    Serial.print(data[i], HEX);
  }
  Serial.println("");

  if(OneWire::crc8(data, 8) == data[8]) {
    Serial.print("GOOD: CRC is valid ! : 0x"); Serial.println(data[8], HEX);
  }
  else {
    Serial.println("BAD: CRC is NOT valid!");
    Serial.print("  Expected: 0x"); Serial.print(OneWire::crc8(data, 8), HEX); Serial.println();
    Serial.print("  Got: 0x"); Serial.print(data[8], HEX); Serial.println();
    return;
  }

  uint16_t TReading = (((uint16_t) data[1]) << 8) + data[0];
  //if(0x8000 & TReading) { // negative
  //  TReading = (TReading ^ 0xffff) + 1; // 2's comp
  //}
  double temperature = (int16_t) TReading;
  Serial.print("temperature: "); Serial.println(temperature/16.0);
  delay(10000);
}

