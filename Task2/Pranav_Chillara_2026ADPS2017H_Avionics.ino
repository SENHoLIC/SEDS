//For using LCD
#include <LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

//STATE Trackers and Variables
int anchorState = 0;
int buttonNew;
int buttonOld;
int LEDState = 0;

enum STATE {
  OPEN_SEA, ANCHOR_DROPPED, STORM, CHARYBDIS, WRECKED
};
STATE currentState = OPEN_SEA;
void stateupdate() {
  switch (currentState) {
    case OPEN_SEA:
    if (anchorState == 1) {
      currentState = ANCHOR_DROPPED;
    }
    else if (lightRead < 512) {
      danger = millis();
      currentState = STORM;
    }
    else if (pingTravelTime != 0 && distance < 100) {
      danger = millis();
      currentState = CHARYBDIS;
    }
    break;
    case ANCHOR_DROPPED:
    if (anchorState == 0) {
      currentState = OPEN_SEA;
    }
    break;
    case STORM:
    if (millis()- danger >= 5000) {
      currentState = WRECKED;
    }
    else if (anchorState == 1) {
      currentState = ANCHOR_DROPPED;
    }
    else if (lightRead >= 512) {
      currentState = OPEN_SEA;
    }

    break;
    case CHARYBDIS:
    if (millis()- danger >= 5000) {
      currentState = WRECKED;
    }
    else if (anchorState == 1) {
      currentState = ANCHOR_DROPPED;
    }
    else if (pingTravelTime == 0 || distance >= 100) {
      currentState = OPEN_SEA;
    }
    break;
    case WRECKED:
    break;
  }
}

//Pins
int LDRPin = 0;
int LEDPin = 11;
int buttonPin = 12;
int buzzPin = 9;
int SIGPin = 10;

//Sensor Inputs
int lightRead;
long pingTravelTime;
long pingTravelDistance;
long distance;

//Time/millis() related
long danger;
long blink = 0;

void setup() {
Serial.begin(9600);
pinMode(buttonPin, INPUT);
buttonOld = digitalRead(buttonPin);
pinMode(LDRPin, INPUT);
pinMode(LEDPin, OUTPUT);
pinMode(buzzPin, OUTPUT);
lcd.begin(16, 2);
}

void loop() {
  //ANCHOR BUTTON logic
  buttonNew = digitalRead(buttonPin);
  if(buttonOld==1 && buttonNew==0) {
    if (anchorState==0) {
      anchorState = 1;
    } else {
      anchorState = 0;
    }
  }
  buttonOld = buttonNew;

//STORM detection
lightRead = analogRead(LDRPin);

//CHARYBDIS detection 
pinMode(SIGPin, OUTPUT);
digitalWrite(SIGPin, LOW);
delayMicroseconds(10);
digitalWrite(SIGPin, HIGH);
delayMicroseconds(10);
digitalWrite(SIGPin, LOW);
pinMode(SIGPin, INPUT);
pingTravelTime = pulseIn(SIGPin, HIGH, 30000);
delay(25); 
pingTravelDistance = pingTravelTime*0.0343;
distance = pingTravelDistance/2;
  
  //updates state machine values from latest sensor readings
  stateupdate();
  
  //STATE logic
  if (currentState == ANCHOR_DROPPED) {
    digitalWrite(LEDPin, LOW);
    noTone(buzzPin);
    lcd.setCursor(0,0);
    lcd.clear();
    lcd.print("ANCHOR_DROPPED");
  }
  else if (currentState == STORM) {
    if (millis() - blink >= 250) {
      blink = millis();
      LEDState = 1 - LEDState;
      digitalWrite(LEDPin, LEDState);
    }
    noTone(buzzPin);
    lcd.setCursor(0,0);
    lcd.clear();
    lcd.print("STORM");
  } 
  else if (currentState == CHARYBDIS) {
    tone(buzzPin, 1000);
    digitalWrite(LEDPin, LOW);
    //for tracking purposes
    Serial.println(distance);
    lcd.setCursor(0,0);
    lcd.clear();
    lcd.print("CHARYBDIS");
  } 
  else if (currentState == WRECKED) {
    noTone(buzzPin);
    digitalWrite(LEDPin, LOW);
    lcd.setCursor(0,0);
    lcd.clear();
    lcd.print("WRECKED");
  } 
  else {
    digitalWrite(LEDPin, LOW);
    noTone(buzzPin);
    lcd.setCursor(0,0);
    lcd.clear();
    lcd.print("OPEN_SEA");
  }

  //For Tracking Purposes
  Serial.println(currentState);
}