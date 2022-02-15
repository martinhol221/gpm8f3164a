
void setup() {

Serial.begin(77000);
Serial.write((byte)0);   Serial.write(0xFF);
Serial.end(); delayMicroseconds(45);   Serial.begin(77000);

}

void loop() { 

Serial.write(0x5A);  Serial.write((byte)0); 
Serial.end(); delayMicroseconds(45);Serial.begin(77000);
Serial.write(0x04); Serial.write(0xFE); 
Serial.end(); delayMicroseconds(45); Serial.begin(77000);
Serial.write(0xE1); Serial.write(0x11); 
Serial.end(); delayMicroseconds(45); Serial.begin(77000);
Serial.write(0x6A); Serial.write(0x12); 
Serial.end(); delayMicroseconds(45); Serial.begin(77000);
Serial.write(0xFF); Serial.write(0x01); 
Serial.end(); delayMicroseconds(45); Serial.begin(77000);
Serial.write(0x8C); 
Serial.end(); delayMicroseconds(352); Serial.begin(77000);

}
