#include <SoftwareSerial.h>

SoftwareSerial SIM800(7, 8); 
String receivedData;        
String phoneNumber;        
String message;             

void setup() {
  Serial.begin(9600);    
  SIM800.begin(9600);    
  delay(100);
}

void loop() {
  if (Serial.available()) {
    receivedData = Serial.readString();  
    
   
    int separatorIndex = receivedData.indexOf(';');  
    if (separatorIndex != -1) {
      phoneNumber = receivedData.substring(0, separatorIndex);  
      message = receivedData.substring(separatorIndex + 1);    

   
      sendSMS(phoneNumber, message);
    }
  }
}

void sendSMS(String phoneNumber, String message) {
  SIM800.println("AT+CMGF=1"); 
  delay(100);
  SIM800.println("AT+CMGS=\"" + phoneNumber + "\"");
  delay(100);
  SIM800.print(message); 
  delay(100);
  SIM800.write(26); 
  delay(1000);
}
