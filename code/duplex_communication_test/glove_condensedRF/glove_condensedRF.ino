//#include <radio.h> //include this library from github
#include <Radio.h> //if you're Colin
#include <CD74HC4067.h> //include this library from github
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> //download library from adafruit website

//defined mux pins from redboard
#define s0 2 
#define s1 6 
#define s2 7 
#define s3 10
#define sig_pin 0

//defined vib motor values
#define PRESSMIN 115
#define PRESSMAX 850

//defined vib motor values
#define VIBMIN 0
#define VIBMAX 4095

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Radio radio(11);
CD74HC4067 demux(s0,s1,s2,s3,sig_pin);

//flex sensors
struct GlovePacket {
  int gThumb, gPointer, gMiddle, gRing, gPinky;
  int gFinger[5] = {gThumb, gPointer, gMiddle, gRing, gPinky};
  int gCheckSum = 0;
} gpkt;

//vibration motors
struct ArmPacket {
  int aThumb, aPointer, aMiddle, aRing, aPinky;
  int aFinger[5] = {aThumb, aPointer, aMiddle, aRing, aPinky};
  int aCheckSum = 0;
} apkt;

void setup() 
{
  delay(1000);
  
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(40);
  
  for (int i = 0; i < 5; i++){
    gpkt.gFinger[i] = 0;
    apkt.aFinger[i] = 0;
  }
  
  start_up_sequence();
}

void loop() {
  //GLOVE PACKET: SENDING FLEX SENSOR VALUES FROM GLOVE TO MOVE SERVO MOTORS ON ROBO ARM
  read_flex_sensors();

  transmit();

  //ARM PACKET: RECEIVING PRESSURE SENSOR VALS FROM ROBO ARM TO MOVE VIB MOTORS
  receive();

  if(is_valid_pkt()) {  
    for (int i = 0; i < 5; i++) {
      pwm.setPWM(i, 0, map(apkt.aFinger[i], PRESSMIN, PRESSMAX, VIBMIN,VIBMAX));
    }
  }
}


void read_flex_sensors() {
  for (int i = 0; i < 5; i++){
    delay(5);
    gpkt.gFinger[i] = demux.read_channel(i);
    delay(5);
    gpkt.gFinger[i] = demux.read_channel(i);
  }
}


void transmit() {
  gpkt.gCheckSum = gpkt.gThumb + gpkt.gPointer + gpkt.gMiddle + gpkt.gRing + gpkt.gPinky;
  radio.rfWrite((uint8_t *) & gpkt, sizeof(GlovePacket));
}


void receive() {
  if(radio.rfAvailable()) {
    radio.rfRead((uint8_t *) & apkt, sizeof(ArmPacket));
  }
}


bool is_valid_pkt() {
  int aCheckSumTot = apkt.aFinger[0] + apkt.aFinger[1] + apkt.aFinger[2] + apkt.aFinger[3] + apkt.aFinger[4];

  return (aCheckSumTot == apkt.aCheckSum);
}


void start_up_sequence() {
  delay(3000);
  test_vibrotactiles();
  delay(3000);
  test_thermoelectrics();
  delay(3000);
}


void test_vibrotactiles() {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 255; j++) {
      pwm.setPWM(i, 0, map(j, 0, 255, VIBMIN,VIBMAX));
      delay(5);
    }
    delay(300);
    for (int j = 255; j >= 0; j--) {
      pwm.setPWM(i, 0, map(j, 0, 255, VIBMIN,VIBMAX));
      delay(5);
    }
    delay(300);
  }

  delay(300);
    
  for (int j = 0; j < 255; j++) {
    for (int i = 0; i < 5; i++) {
      pwm.setPWM(i, 0, map(j, 0, 255, VIBMIN,VIBMAX));
      
    }    
    delay(10);
  }
  delay(300);
  for (int j = 255; j >= 0; j--) {
    for (int i = 0; i < 5; i++) {
      pwm.setPWM(i, 0, map(j, 0, 255, VIBMIN,VIBMAX));
      
    } 
    delay(10);   
  }
  delay(300);
}

void test_thermoelectrics() {
  
}
