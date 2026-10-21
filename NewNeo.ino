#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "VoiceRecognitionV3.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

/* for start gps */
  const int RXPin = 9, TXPin =8;
  const uint32_t GPSBaud = 9600; //Default baud of NEO-6M is 9600

  TinyGPSPlus gps; // the TinyGPS++ object
  SoftwareSerial gpsSerial (RXPin, TXPin); // the serial interface to the GPS device
  String lati = "";
  String longi = "";
  String url = "";
  int x =1; //conter to for gps

/* for end gps */

/* for start gsm */
  #define rxPin 4
  #define txPin 5
  SoftwareSerial sim900(rxPin,txPin); 
  String textMessage;
  String buff;
/* for end gsm */

/* button command*/

  int buzzer= 7;     // buzzer
  int buzzer_2= 6;     // buzzer

  int push_button=10;     // end the call
  int push_button_2=11;  //make a call
  int push_button_3=12;  // awnasr the call
  int push_button_4=13;  // send a message


    // to put the value of the the button
  int push_button_stat;
  int push_button_stat_2;
  int push_button_stat_3;
  int push_button_stat_4;


/* end button command*/

/* for voice*/

  VR myVR(3,2);    // 2:RX 3:TX, you can choose your favourite pins.

  uint8_t records[7]; // save record
  uint8_t buf[64];

  //inetial youer command 
  #define onRecord    (0)
  #define offRecord   (1)
  #define callX   (2)
  #define cally  (3)


  void printSignature(uint8_t *buf, int len)
  {
    int i;
    for(i=0; i<len; i++){ 
      if(buf[i]>0x19 && buf[i]<0x7F){ 
        Serial.write(buf[i]); 
      } 
      else{ 
        Serial.print("["); 
        Serial.print(buf[i], HEX); 
        Serial.print("]"); 
      } 
    } 
  } 

  void printVR(uint8_t *buf)
  {
    Serial.println("VR Index\tGroup\tRecordNum\tSignature");

    Serial.print(buf[2], DEC);
    Serial.print("\t\t");

    if(buf[0] == 0xFF){
      Serial.print("NONE");
    }
    else if(buf[0]&0x80){
      Serial.print("UG ");
      Serial.print(buf[0]&(~0x80), DEC);
    }
    else{
      Serial.print("SG ");
      Serial.print(buf[0], DEC);
    }
    Serial.print("\t");

    Serial.print(buf[1], DEC);
    Serial.print("\t\t");
    if(buf[3]>0){
      printSignature(buf+4, buf[3]);
    }
    else{
      Serial.print("NONE");
    }
    Serial.println("\r\n");
  }
/* for voice*/

/* for axccelometer */
  Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
/* for axccelometer */

/*start ultrea sonic*/
  const int trigPin2 = A2;

  const int echoPin2 = A3;
  
  long duration2;
  
  int distance2;

/*end ultea sonic*/


void setup(){
  /* for start gps */
    gpsSerial.begin(9600);
    gpsSerial.begin(GPSBaud);
    Serial.println(F("Arduino - GPS module"));

  /* for end gps */


  /* button command*/
    pinMode(buzzer, OUTPUT);
        pinMode(buzzer_2, OUTPUT);

    pinMode(push_button, INPUT);
    pinMode(push_button_2, INPUT);
    pinMode(push_button_3, INPUT);
    pinMode(push_button_4, INPUT);

  /* end button command*/

  /* for start gsm */

    //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
    Serial.begin(9600);
    
    //Begin serial communication with Arduino and sim900
    sim900.begin(9600);

    Serial.println("Initializing...");
    
    sim900.println("AT");
    waitForResponse();

    sim900.println("ATE1");
    waitForResponse();

    sim900.println("AT+CMGF=1");
    waitForResponse();

    sim900.println("AT+CNMI=1,2,0,0,0");
    waitForResponse();

    // sim900.print("AT+CNMI=2,2,0,0,0\r");
    // waitForResponse();
  /* for end gsm */



  /* for starting voice */
    myVR.begin(9600);
    Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
    
   
      
    if(myVR.clear() == 0){
      Serial.println("Recognizer cleared.");
    }else{
      Serial.println("Not find VoiceRecognitionModule.");
      Serial.println("Please check connection and restart Arduino.");
      while(1);
    }

    // all your comand 
    
    if(myVR.load((uint8_t)onRecord) >= 0){
      Serial.println("sms");
    }
    
    if(myVR.load((uint8_t)offRecord) >= 0){
      Serial.println("call");
    }
    if(myVR.load((uint8_t)callX) >= 0){
      Serial.println("callX loaded");
    }
        if(myVR.load((uint8_t)cally) >= 0){
      Serial.println("cally loaded");
    }
  /* for ending voice */

  /*  for start axccelometer */

    if(!accel.begin()) {
      Serial.println("Could not find a valid ADXL345 sensor, check wiring!");
      while(1);
    }
  /* for end axccelometer */

  /*start ultea sonic*/
    pinMode(trigPin2, OUTPUT);
 
    pinMode(echoPin2, INPUT);
  /*end ulter sonic*/




}

void loop(){

   /*for start gps*/
    gpsSerial.listen();
                    //  Serial.println("3333333333333");
                    delay(100);

      while (gpsSerial.available() > 0) {
              //  Serial.println("1111111111111111");

        if (gps.encode (gpsSerial.read())) {

          if (gps.location.isValid()) {
            //  Serial.print(F("- latitude: "));
            lati =String(gps.location.lat(), 6);
            // Serial.print(F("- longitude: "));
            longi = String(gps.location.lng(), 6);
            url = "https://www.google.com/maps/place/" +lati+ "+" + longi ;
               Serial.println(url);
             x=1000000;
          } 
          else {
            Serial.println(F("- location: INVALID"));
          }
        }
      }
    
  /*for end gps*/


  /* button command*/
    // save the value of button

      push_button_stat = digitalRead(push_button);
      push_button_stat_2 = digitalRead(push_button_2);
      push_button_stat_3 = digitalRead(push_button_3);
      push_button_stat_4 = digitalRead(push_button_4);

  /*  end button command*/


  /*for start gps*/
    sim900.listen();
    delay(10);
    if(sim900.available()){
      buff = sim900.readString();
      Serial.println(buff);  
      textMessage =buff;
      //to send the loaction 
      if(textMessage.indexOf("LOCKION")>=0){
        Serial.print(url);  
                send_sms();
        textMessage = "";   
        digitalWrite(buzzer, HIGH);
        delay(100);
        digitalWrite(buzzer, LOW);
        delay(100);
        digitalWrite(buzzer, HIGH);
        delay(100);
        digitalWrite(buzzer, LOW);
      }
    }

  /*for end gps*/

  /* The button comands */
    // end the call 
      if (push_button_stat == 0){
        digitalWrite(buzzer, HIGH);
        sim900.println("ATH");  
      }
      else {
        digitalWrite(buzzer, LOW);
      }

    // make a call 
      if (push_button_stat_2 == 0){
        make_call();
        digitalWrite(buzzer, HIGH);
        delay(100);
        digitalWrite(buzzer, LOW);
        delay(100);
        digitalWrite(buzzer, HIGH);
        delay(100);
        digitalWrite(buzzer, LOW);
      }

    // aswser the call 
      if (push_button_stat_3 == 0){
        digitalWrite(buzzer, HIGH);
        sim900.println("ATA");  
      }
      else {
        digitalWrite(buzzer, LOW);
      }

    // send a messge
      if (push_button_stat_4 == 0){
         Serial.println(url);

          sim900.print("AT+CMGS=\"+9647727136806\"\r");
          delay(1000);
          while(sim900.available()){
            Serial.println(sim900.readString());
          }
          sim900.read();

          sim900.print(url);
          sim900.write(0x1A);

          delay(1000);
           while(sim900.available()){
            Serial.println(sim900.readString());
          }
          sim900.read();
        digitalWrite(buzzer, HIGH);
        delay(100);
        digitalWrite(buzzer, LOW);
        delay(100);
        digitalWrite(buzzer, HIGH);
        delay(100);
        digitalWrite(buzzer, LOW);
      }     

  /*end the comands*/

  
  /*start voice command*/
    myVR.listen();
    int ret;
    ret = myVR.recognize(buf, 50);
    if(ret>0){
      switch(buf[1]){
        case onRecord:
          digitalWrite(buzzer, HIGH);
          delay(100);
          digitalWrite(buzzer, LOW);
          delay(100);
          digitalWrite(buzzer, HIGH);
          delay(100);
          digitalWrite(buzzer, LOW);

          Serial.println("21333333333333333333");
          Serial.println(url);

          sim900.print("AT+CMGS=\"+9647727136806\"\r");
          delay(1000);
          while(sim900.available()){
            Serial.println(sim900.readString());
          }
          sim900.read();

          sim900.print(url);
          sim900.write(0x1A);

          delay(1000);
           while(sim900.available()){
            Serial.println(sim900.readString());
          }
          sim900.read();
          break;

        case offRecord:
          digitalWrite(buzzer, HIGH);
          delay(100);
          digitalWrite(buzzer, LOW);
          delay(100);
          digitalWrite(buzzer, HIGH);
          delay(100);
          digitalWrite(buzzer, LOW);
          Serial.println("444444444444444444444444444444");
          Serial.println(url);
          Serial.println("url");
          sim900.println("ATD+9647727136806;");
          delay(1000);
          while(sim900.available()){
            Serial.println(sim900.readString());
          }
          sim900.read();

          break;

      }
      /** voice recognized */
      printVR(buf);
    }

  /*end voice command*/

  /* for start axccelometer */
    sensors_event_t event;
    accel.getEvent(&event);
    // Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print(" m/s^2\t");
    // Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print(" m/s^2\t");
    // Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print(" m/s^2\t");
    // Serial.println("");
    if (event.acceleration.x >=3 || event.acceleration.x <= -3){
       digitalWrite(buzzer, HIGH);
      }
    else {
        digitalWrite(buzzer, LOW);
    }
  /* for end axccelometer */

  /*start ulter sonic*/
     delay(20);
     digitalWrite(trigPin2, LOW);
     delayMicroseconds(2);
     digitalWrite(trigPin2, HIGH);
     delayMicroseconds(10);
     digitalWrite(trigPin2, LOW);
     duration2 = pulseIn(echoPin2, HIGH);
     distance2 = duration2 * 0.034 / 2;
    //  Serial.print("Distance2: ");
    //  Serial.println(distance2);
     if (distance2 <= 40){
        digitalWrite(buzzer_2, HIGH);
      }
      else {
        digitalWrite(buzzer_2, LOW);
      }
     
  /* end ulter sonic*/

  // x=x-1; // counter for gps 
}





void send_sms(){
  sim900.print("AT+CMGS=\"+9647727136806\"\r");
  waitForResponse();
  
  sim900.print(url);
  sim900.write(0x1A);
  waitForResponse();
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
}


void make_call(){

  sim900.println("ATD+9647727136806;");
  waitForResponse();
      digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
}

void waitForResponse(){
  delay(1000);
  while(sim900.available()){
    Serial.println(sim900.readString());
  }
  sim900.read();
}


