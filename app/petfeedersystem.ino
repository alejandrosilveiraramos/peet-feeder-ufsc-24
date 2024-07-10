#include <Servo.h>

const int digitalSensorPin = 2;
const int dispenseAngle = 45;
const int startAngle = 90; 

const int servoPin = 9;

const int waitTimeShort = 500;
const int waitTimeMiddle = 1000;
const int waitTimeLong = 5000;

const unsigned long dispensingInterval = 15000UL; 

Servo myservo;

unsigned long lastDispenseTime = 0; 

void initializeSystem();
void handleDispensing();

bool isSensorDispense();
bool isTimeDispense();

void moveServoSmoothly(int startAngle, int endAngle, int stepDelay);

void setup() {
    initializeSystem();
    lastDispenseTime = millis() - dispensingInterval; 
}

void loop() {
    handleDispensing();
    delay(waitTimeShort);
}

void initializeSystem() {
    Serial.begin(9600);
    pinMode(digitalSensorPin, INPUT);
    myservo.attach(servoPin);
    myservo.write(startAngle);
}

void handleDispensing() {    
    if (isSensorDispense() && isTimeDispense()) {
        moveServoSmoothly(startAngle, dispenseAngle, waitTimeMiddle / 45);
        delay(waitTimeLong);
        moveServoSmoothly(dispenseAngle, startAngle, waitTimeMiddle / 45);
        lastDispenseTime = millis(); 
        Serial.println("Food dispensed. Timer reset.");
    }
}

void moveServoSmoothly(int startAngle, int endAngle, int stepDelay) {
    if (startAngle < endAngle) {
        for (int angle = startAngle; angle <= endAngle; angle++) {
            myservo.write(angle);
            delay(stepDelay);
        }
    } else {
        for (int angle = startAngle; angle >= endAngle; angle--) {
            myservo.write(angle);
            delay(stepDelay);
        }
    }
}

bool isTimeDispense() {
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - lastDispenseTime;
    Serial.print("Elapsed Time: ");
    Serial.println(elapsedTime);
    if (elapsedTime >= dispensingInterval) {
        return true;
    }  
    Serial.print("Cannot dispense, wait for ");
    Serial.print(dispensingInterval - elapsedTime);
    Serial.println(" milliseconds.");
    return false;
}

bool isSensorDispense() {
    int sensorState = digitalRead(digitalSensorPin);
    Serial.print("Sensor State: ");
    Serial.println(sensorState);
    if (sensorState == HIGH) {
        return true;
    } 
    Serial.println("No movement detected, waiting for some movement");
    return false;
}
