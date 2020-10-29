#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

HUSKYLENS huskylens;
SoftwareSerial mySerial(4, 0);
//HUSKYLENS green line >> Pin 10; blue line >>Pin11
void printResult(HUSKYLENSResult result);

void setup() {
    pinMode(2, OUTPUT);
    Serial.begin(115200);
    mySerial.begin(9600);
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
      delay(500);
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
        if (result.xCenter > 160){
            Serial.println(F("Obeject's Position : RIGHT"));
            digitalWrite(2,HIGH);
            delay(500);
        }
        else if (result.xCenter == 160){
            Serial.println(F("Obeject's Position : MIDDLE"));
            delay(500);
        }
        else {
            Serial.println(F("Obeject's Position : LEFT"));
            digitalWrite(2,LOW);
            delay(500);
        }
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
        delay(500);
    }
    else{
        Serial.println("Object unknown!");
        delay(500);
    }
}
