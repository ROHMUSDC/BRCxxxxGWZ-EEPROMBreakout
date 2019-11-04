#include <Wire.h>

//unsigned char WriteData[6] = {0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
unsigned char WriteData[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  //Reset to Defaults
unsigned char ReadData[6] = {0, 0, 0, 0, 0, 0};
byte rc;

/*----- EEPROM Test Procedure -----

0. Run Only Read Test Section with EEPROM not attached. PASS if I2C Write/Read Fails... [PASS]
1. Run Only Read Test Section. PASS if return is all 0x00 in all contents...
2. Run Both Write and Read Test Sections... PASS if return from Read Test is A, B, C...
3. Cycle Power. Run only Read Test Section... Pass if return from Read Test is A, B, C...

-----------------------------------*/

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  } 
  Serial.print(F("Application Started!!\r\n"));
  delay(2000);
  Serial.print(F("Test Started!!\r\n"));

  //------------------- Write Test Section ---------------------------------------------------
  //Dev Addr: 0x54 (high), 0x50 (low)
  rc = I2C_write(0x50, 0x0F, 0x0F, WriteData, sizeof(WriteData));
  if (rc != 0) {
    Serial.print(F("I2C Write Failed... can't access EEPROM\r\n"));
  }
  else{
    Serial.print(F("I2C Write Passed... EEPROM write OK\r\n"));
  }
  //------------------- End Write Test Section -----------------------------------------------
  
  delay(3000);

  //------------------- Read Test Section ----------------------------------------------------
  rc = I2C_read(0x50, 0x0F, 0x0F, ReadData, sizeof(WriteData));
  if (rc != 0) {
    Serial.print(F("I2C Read Failed... can't access EEPROM\r\n"));
  }
  else{
    Serial.print(F("I2C Read Passed... EEPROM Read OK\r\n"));
    Serial.print(F("Return Contents from 0F0F = "));
    Serial.print(ReadData[0], HEX);
    Serial.print(F("h, "));
    Serial.print(ReadData[1], HEX);
    Serial.print(F("h, "));
    Serial.print(ReadData[2], HEX);
    Serial.print(F("h, "));
    Serial.print(ReadData[3], HEX);
    Serial.print(F("h, "));
    Serial.print(ReadData[4], HEX);
    Serial.print(F("h, "));
    Serial.print(ReadData[5], HEX);
    Serial.print(F("h\r\n"));
    Serial.print(F("EEPROM Test Done... \r\n"));
  }
  //------------------- End Read Test Section ------------------------------------------------

  
}

void loop() {
  // Nothing Looping.... Testing all by checking the through the initial section...
}

byte I2C_write(int device_address, unsigned char memory_address1, unsigned char memory_address2, unsigned char *data, unsigned char size)
{
  byte rc;

  Wire.beginTransmission(device_address);
  Wire.write(memory_address1);
  Wire.write(memory_address2);
  Wire.write(data, size);
  rc = Wire.endTransmission();
  
  return (rc);
}

byte I2C_read(int device_address, unsigned char memory_address1, unsigned char memory_address2, unsigned char *data, int size)
{
  byte rc;
  unsigned char cnt;

  Wire.beginTransmission(device_address);
  Wire.write(memory_address1);
  Wire.write(memory_address2);
  rc = Wire.endTransmission(false);
  if (rc != 0) {
    return (rc);
  }

  Wire.requestFrom(device_address, size, true);
  cnt = 0;
  while(Wire.available()) {
    data[cnt] = Wire.read();
    cnt++;
  }

  return (rc);
}
