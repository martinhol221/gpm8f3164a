#define AC_pin A0
byte speed_byte[] =  {0x5A,0x00,0xB0,0x00,0x03,0x11,0x6A,0x12,0xFF,0x01};

void setup() {
pinMode(AC_pin, INPUT);

Serial.begin(77000);
Serial.write((byte)0);   Serial.write(0xFF);
Serial.end(); delayMicroseconds(45);   Serial.begin(77000);
}

void loop() { 
int acceleratorValue = analogRead(AC_pin);  
acceleratorValue= map(acceleratorValue,174,868,0,255);
acceleratorValue = constrain(acceleratorValue, 0,255);
  
Serial.write(speed_byte[0]); // const
Serial.write(speed_byte[1]); // transverse slope, in a horizontal position = 00
Serial.end(); delayMicroseconds(45);Serial.begin(77000);
Serial.write(speed_byte[2]); // ?
Serial.write(speed_byte[3]); // ?
Serial.end(); delayMicroseconds(45); Serial.begin(77000);
Serial.write(acceleratorValue); // speed
Serial.write(speed_byte[5]); // const
Serial.end(); delayMicroseconds(45); Serial.begin(77000);
Serial.write(speed_byte[6]); // const
Serial.write(speed_byte[7]); // const
Serial.end(); delayMicroseconds(45); Serial.begin(77000);
Serial.write(speed_byte[8]); // const
Serial.write(speed_byte[9]); // optical pressure sensor 00 or O1
Serial.end(); delayMicroseconds(45); Serial.begin(77000);
Serial.write(speed_byte[1]^speed_byte[2]^speed_byte[3]^acceleratorValue^speed_byte[5]^speed_byte[6]^speed_byte[7]^speed_byte[8]^speed_byte[9]); 
Serial.end(); delayMicroseconds(352); Serial.begin(77000);

}

