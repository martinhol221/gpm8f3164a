
byte speed_3_byte[] =  {0x94,0xB8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
byte speed_4_byte[] =  {0xFD,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
byte speed_5_byte[] =  {0x39,0x99,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
byte speed_11_byte[] = {0xC7,0xB7,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

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



#define FLA_sensor 39
#define RSV_sensor 36
#define FAN_sensor 34
#define FDR_sensor 35
#define FDR_motor 18
#define CLN_pin 19
#define FAN_pin 33
#define FLUEGAS_pin 32



unsigned int FLA[4]; RSV[4]; half_read;
unsigned long COUNT_FAN; COUNT_FDR; 

const int FanChannel = 0;
const int FluegasChannel = 1;
      int freq_pwm = 300;

pinMode(FLA_sensor, INPUT);
pinMode(RSV_sensor, INPUT);
pinMode(FAN_sensor, INPUT);
pinMode(FDR_sensor, INPUT);
pinMode(FDR_motor, OUTPUT);

attachInterrupt(digitalPinToInterrupt(FAN_sensor), fan_steep_function, FALLING); // RISING,  
attachInterrupt(digitalPinToInterrupt(FDR_sensor), FDR_OFF_function, CHANGE); // RISING, 

ledcSetup(FanChannel, freq_pwm, 8); 
ledcAttachPin(FAN_pin, FanChannel);


ledcSetup(FluegasChannel, freq_pwm, 8); 
ledcAttachPin(FLUEGAS_pin, FluegasChannel);


server.on("/test", handleTest);
server.on("/testing", handleTesting);



void fan_steep_function() {COUNT_FAN++;}


void FDR_ON_function(int half_count) {half_read = half_count;         digitalWrite(FDR_motor, HIGH);}
void FDR_OFF_function()              {half_read--; if (half_read < 1) digitalWrite(FDR_motor,  LOW);}


void read_pins_function() {FLA[3]=FLA[2]; FLA[2]=FLA[1]; FLA[1]=analogRead(FLA_sensor); FLA[0]=FLA[1]+FLA[2]+FLA[3]/3;
                           RSV[3]=RSV[2]; RSV[2]=RSV[1]; RSV[1]=analogRead(RSV_sensor); RSV[0]=RSV[1]+RSV[2]+RSV[3]/3;}



void  handleTesting(){ String http = "Данные не верны";
                   
if (server.arg("fan_pwm") !="")           int fan_pwm = constrain(server.arg("fan_pwm").toInt(), 0, 255);      ledcWrite(FanChannel, fan_pwm);      http = "Установлено ШИМ вентилятора "+String(fan_pwm);
if (server.arg("fluegas_pwm") !="")   int fluegas_pwm = constrain(server.arg("fluegas_pwm").toInt(), 0, 255);  ledcWrite(FanChannel, fluegas_pwm);  http = "Установлено ШИМ дымососа "+String(fluegas_pwm);
if (server.arg("fdr_steep") !="")       int fdr_steep = constrain(server.arg("fdr_steep").toInt(), 0, 255);    FDR_ON_function(fdr_steep);          http = "Задано поуоборотов шнека "+String(fdr_steep);
if (server.arg("freq_pwm") !="")             freq_pwm = constrain(server.arg("freq_pwm").toInt(), 10, 10000);                                       http = "Установлена частота ШИМ "+String(freq_pwm);
                
server.send(200, "text/html", head + http + footer);}   



void handleTest(){String http = "<div>" + form_save("Тестирование","/test")
+ input_txt("Частота ШИМ для вентиляторов", "freq_pwm", String(freq_pwm), 1, 10)
+ input_txt("ШИМ для FAN", "fan_pwm", "0-255", 1, 10)
+ input_txt("ШИМ для FLUEGAS FAN", "fan_pwm", "0-255", 1, 10)      
+ input_txt("Задать движение шнека полуоборотов", "fdr_steep", "0-255", 1, 10)
+ submit("Начать тест");
server.send(200, "text/html", head + http + homeButton + footer);}






String input_txt(String lable, String name, String placeholder, int pat, int maxlength) { String pattern;

switch (pat){
case 0:  pattern = "pattern=\"^[a-zA-Z\\.0-9]+$\"";  break;  // все латинские
case 1:  pattern = "pattern=\"[0-9]{0,10}\"";        break;  // int  [0-9]{0,10}
case 2:  pattern = "pattern=\"\\d+(\\.\\d{2})?\"";   break;  // float \\d+(\\.\\d{2})? 
case 3:  pattern = "pattern=\"\\+[0-9]{9,13}\"";     break;  // Tel
default: pattern = "";}

String i = "<label for=\"fname\">"+lable+"</label><input type=\"text\" "+pattern+" id=\"fname\" name=\""+name+"\" maxlength=\""+String(maxlength)+"\" placeholder=\""+placeholder+"\">";
return i;}

String form_action(String lable, String action) {String i = "<b>"+lable+"</b><br><br><form action=\""+action+"\">"; return i;}
String submit(String opt) {String i = "<input type=\"submit\" value=\""+opt+"\"></form>"; return i;}







