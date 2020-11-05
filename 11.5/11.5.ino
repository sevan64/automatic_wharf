#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include <ESP32Servo.h>

HUSKYLENS huskylens;
SoftwareSerial mySerial(4, 0);
//HUSKYLENS green line >> Pin 10; blue line >>Pin11
Servo servo1;
Servo servo2;
Servo servo3;
const int maxUs = 1900;
const int minUs = 1100;
const int servo1Pin = 19;//表記はD12 右後ろ 正の方向が後退 1100で後退
const int servo1Period = 50;
const int servo2Pin = 23;//表記はD11 前　正の方向が前身 1100で前進
const int servo2Period = 50;
const int servo3Pin = 18;//表記はD13 左後ろ 正の方向が後退 1100で後退
const int servo3Period = 50;
int servo1Us = 1500;
int servo2Us = 1500;
int servo3Us = 1500;
int camera_width = 320;
int camera_hight = 240;

void printResult(HUSKYLENSResult result);

void setup() {
    pinMode(2, OUTPUT);
    Serial.begin(115200);
    mySerial.begin(9600);
    servo1.setPeriodHertz(servo1Period);
    servo1.attach(servo1Pin, minUs, maxUs);

    servo2.setPeriodHertz(servo2Period);
    servo2.attach(servo2Pin, minUs, maxUs);

    servo3.setPeriodHertz(servo3Period);
    servo3.attach(servo3Pin, minUs, maxUs);

    servo1.writeMicroseconds(1500);
    servo2.writeMicroseconds(1500);
    servo3.writeMicroseconds(1500);
    while (!huskylens.begin(mySerial))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please retcheck the \"Protocol Type\" in HUSKYLENS (General Setting>>Protocol Type>>Serrial 9600)"));
        Serial.println(F("2.Please retcheck the connection."));
        delay(100);
    }
}

void loop() {
    if (!huskylens.request()){
      Serial.println(F("Fail to request data from HUSKYLENS, retcheck the connection!"));
      delay(500);
    }
    else if(!huskylens.isLearned()){
      Serial.println(F("Nothing learned, press learn button on HUSKYLENS to leran one!"));
      delay(500);
    }
    else if(!huskylens.available()){
      Serial.println(F("No block or arrow apears on the screen!"));
      servo1.writeMicroseconds(1900);
      servo2.writeMicroseconds(1500);
      servo3.writeMicroseconds(1100);
      delay(200);
    }
    else
    {
        while (huskylens.available())
        {
            HUSKYLENSResult result = huskylens.read();
            printResult(result);
        }
    }
}

void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
      if (result.ID == 2){
            if (result.xCenter > 165){
                Serial.println(F("Obeject's 2 Position : RIGHT"));
                servo1.writeMicroseconds(1100);
                servo2.writeMicroseconds(1100);
                servo3.writeMicroseconds(1900);
                delay(700);
            }
            else if (result.xCenter < 165 && result.xCenter > 155){
                Serial.println(F("Obeject's 2 Position : MIDDLE"));
                servo1.writeMicroseconds(1900);
                servo2.writeMicroseconds(1100);
                servo3.writeMicroseconds(1900);
                delay(700);
            }
            else {
                Serial.println(F("Obeject's 2 Position : LEFT"));
                servo1.writeMicroseconds(1900);
                servo2.writeMicroseconds(1100);
                servo3.writeMicroseconds(1100);
                delay(700);
            }
      }
      else:
          if (result.ID == 1){
              if (result.xCenter > 170){
                  Serial.println(F("Obeject's 1 Position : RIGHT"));
                  servo1.writeMicroseconds(1100);
                  servo2.writeMicroseconds(1100);
                  servo3.writeMicroseconds(1900);
                  delay(500);
                }
              else if (result.xCenter < 170 && result.xCenter > 150){
                  Serial.println(F("Obeject's 1 Position : MIDDLE"));
                  servo1.writeMicroseconds(1900);
                  servo2.writeMicroseconds(1100);
                  servo3.writeMicroseconds(1900);
                  delay(500);
                }
              else {
                  Serial.println(F("Obeject's 1 Position : LEFT"));
                  servo1.writeMicroseconds(1900);
                  servo2.writeMicroseconds(1100);
                  servo3.writeMicroseconds(1100);
                  delay(500);
                }
          }
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
        delay(500);
    }
    else{
        Serial.println("Object unknown!");
        servo1.writeMicroseconds(1900);
        servo2.writeMicroseconds(1500);
        servo3.writeMicroseconds(1100);
        delay(500);
    }
}
