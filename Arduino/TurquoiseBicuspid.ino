#include <SoftwareSerial.h>

SoftwareSerial sSerial(9, 10); // TX, RX
String BUFFER = "";
char DELIMETER = ':';
int LED = 13;

void setup()
{
   pinMode(LED, OUTPUT); 
  
   Serial.begin(9600);
   Serial.println("Type AT commands!");

   // JY-MCU v1.08 defaults to 9600.
   sSerial.begin(9600);
   sSerial.write("AT+BAUD4");
   sSerial.write("AT+NAMETurquoiseBicuspid");
   sSerial.write("AT+PIN1234");
}

void loop()
{
  String type = "";
  int time = 0;
  int looper = 0;
  // read from bluetooth
  if(sSerial.available()) {
    while(sSerial.available()) {
      BUFFER += (char)sSerial.read();
    }
    
    String buff = "";
    int parsed = 0;
    for(int i=0; i<BUFFER.length(); i++) {
      buff += BUFFER[i];
      
      // delimeter
      if(BUFFER[i] == ':') {
        buff = "";
      }
      
      // grab value
      if(BUFFER[i+1] == ':') {
        if(parsed == 0) {
          type = buff;
          parsed++;
        }
        else if(parsed >= 1) {
          looper = buff.toInt();
        }
        buff = "";
      }
      
      // grab last value
      if(i == (BUFFER.length()-1)) {
        time = buff.toInt();
      }
    }
    BUFFER = "";
    Serial.println(type+":"+time+":"+looper);
    
    if(type == "blink") {
      blink(time, looper);
    }
    else if(type == "pulse") {
      blink(time, looper);
    }
    
  }
  
  // AT commands
  if(Serial.available()){
    delay(10);
    sSerial.write(Serial.read());
  }
}

void blink(int time, int looper) {
  for(int i=0; i<looper; i++) {
    digitalWrite(LED, HIGH);
    delay(time);
    digitalWrite(LED, LOW);
    delay(time);
  }
}
