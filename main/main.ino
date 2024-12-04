/*
 * Based on the documentation on Adruino
 * Comment this out to work on Tinkercad!
 */

//#include <Arduino.h>

/*
 * Protocol
 */
#define DECODE_NEC          
//#define DECODE_HASH         // special decoder for all protocols
//#define DEBUG               

/*
 * Libraries
 */
#include <IRremote.hpp>
#include <Wire.h>
#include <LiquidCrystal.h>

/*
 * Define PINS
 */
// IR Receiver
const int IR_RECEIVE_PIN = 2;
const int IR_SEND_PIN = 3;

// RGB LED
const int R_PIN = 10;
const int G_PIN = 9;
const int B_PIN = 6;

// Ultrasonic sensor
const int TRIG_PIN = 5;
const int ECHO_PIN = 4;

// LCD screen
const int LCD_rs = 12;
const int LCD_en = 11;
const int LCD_d4 = 13;
const int LCD_d5 = 8;
const int LCD_d6 = 7;
const int LCD_d7 = 3;

// Motors
const int MTR_L_FWD = A1;   //in1
const int MTR_L_BACK = A2;  //in2
const int MTR_R_FWD = A3;   //in3
const int MTR_R_BACK = A4;  //in4


/*
 * Remote vars on BOARD
 * Comment this out to work on Tinkercad
 */
// /*
const int IR_POWER_OFF = 0x45;
const int IR_BUTTON_0 = 0x16;
const int IR_BUTTON_1 = 0xC;
const int IR_BUTTON_2 = 0x18;
const int IR_BUTTON_3 = 0x5E;
const int IR_ARROW_DOWN = 0x7;
const int IR_ARROW_UP = 0x9;
const int IR_BUTTON_4 = 0x8;
const int IR_BUTTON_5 = 0x1C;
const int IR_BUTTON_6 = 0x5A;
const int IR_BUTTON_7 = 0x42;
const int IR_BUTTON_8 = 0x52;
const int IR_BUTTON_9 = 0x4A;
const int IR_VOL_UP = 0x46;
const int IR_VOL_DOWN = 0x15;
const int IR_BUTTON_NEXT = 0x43;
const int IR_BUTTON_PRE = 0x44;
const int IR_BUTTON_PAUSE = 0x40;
const int IR_FUNC_STOP = 0x47;
const int IR_BUTTON_EQ = 0x19;
const int IR_BUTTON_ST = 0xD;
// */

/*
 * Remote vars on TINKERCAD
 * Comment this out to work on board
 */

/*
const int IR_POWER_OFF = 0x00;
const int IR_BUTTON_0 = 0xC;
const int IR_BUTTON_1 = 0x10;
const int IR_BUTTON_2 = 0x11;
const int IR_BUTTON_3 = 0x12;
const int IR_ARROW_DOWN = 0x8;
const int IR_ARROW_UP = 0xA;
const int IR_BUTTON_4 = 0x14;
const int IR_BUTTON_5 = 0x15;
const int IR_BUTTON_6 = 0x16;
const int IR_BUTTON_7 = 0x18;
const int IR_BUTTON_8 = 0x19;
const int IR_BUTTON_9 = 0x1a;
const int IR_VOL_UP = 0x1;
const int IR_VOL_DOWN = 0x9;
const int IR_BUTTON_NEXT = 0x6;
const int IR_BUTTON_PRE = 0x4;
const int IR_BUTTON_PAUSE = 0x5;
const int IR_FUNC_STOP = 0x2;
const int IR_BUTTON_EQ = 0xD;
const int IR_BUTTON_ST = 0xE;
// */


/*
 * Variables
 */
int rBright = 255;
int gBright = 255;
int bBright = 255;

float dimFactor = 1;

// LCD setup
LiquidCrystal lcd(12, 11, A0, 8, 7, 3);


void setup() {
    // Setup ultrasonic sensor
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // Start Serial
    Serial.begin(115200);
    while (!Serial);

    // Setup LED
    pinMode(R_PIN, OUTPUT);
    pinMode(G_PIN, OUTPUT);
    pinMode(B_PIN, OUTPUT);
    
    // Setup Wheels
    pinMode(MTR_L_FWD, OUTPUT);
    pinMode(MTR_L_BACK, OUTPUT);
    pinMode(MTR_R_FWD, OUTPUT);
    pinMode(MTR_R_BACK, OUTPUT);
    // pinMode(B_PIN, OUTPUT);

    // Info about program
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    // Start the receiver
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

    // Start the LCD
    lcd.begin(16,2);
  	delay(100);
  	// lcd.print("Cat!");
    displayEmotion("sad");

    Serial.print(F("Ready to receive IR signals of protocols: "));
    printActiveIRProtocols(&Serial);
}

void loop() {
    // Handle IR Receiver
    if (IrReceiver.decode()) {
        // Handle IR
        handleRemoteCommand(IrReceiver.decodedIRData.command);

        IrReceiver.resume(); // Enable receiving of the next IR frame
        Serial.println(IrReceiver.lastDecodedCommand, HEX);
    }

    // Handle Ultrasonic Sensor
    float distance = pollUltrasonicSensor();
    if (distance < 3) {
        Serial.println("Purr");
        petting();
    } else if(distance < 15) {   // purr???
      displayEmotion("happy");
    } else{
      displayEmotion("sad");
    }
}

float pollUltrasonicSensor() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    float duration = pulseIn(ECHO_PIN, HIGH);
    float distance = (duration * 0.0343) / 2;
    delay(100);
    return distance;
}

void moveWheels(int direction) {
  	if (direction == 0) {  // Up (Forward)
    	digitalWrite(MTR_L_FWD, LOW);
      digitalWrite(MTR_L_BACK, HIGH);
    	digitalWrite(MTR_R_FWD, HIGH);
      digitalWrite(MTR_R_BACK, LOW);
      stopCar();
    }
  	else if (direction == 1) {  // Left
    	// digitalWrite(MTR_L_FWD, HIGH);
      // digitalWrite(MTR_L_BACK, LOW);
    	digitalWrite(MTR_R_FWD, HIGH);
      digitalWrite(MTR_R_BACK, LOW);
      stopCar();
    }
  	else if (direction == 2) {  // Right
    	digitalWrite(MTR_L_FWD, LOW);
      digitalWrite(MTR_L_BACK, HIGH);
    	// digitalWrite(MTR_R_FWD, LOW);
      // digitalWrite(MTR_R_BACK, HIGH);
      stopCar();
    }
  	else if (direction == 3) {  // Down (Backward)
    	digitalWrite(MTR_L_FWD, HIGH);
      digitalWrite(MTR_L_BACK, LOW);
    	digitalWrite(MTR_R_FWD, LOW);
      digitalWrite(MTR_R_BACK, HIGH);
      stopCar();
    }
    else if (direction == 4){ // Pause
      digitalWrite(MTR_L_FWD, LOW);
      digitalWrite(MTR_L_BACK, LOW);
    	digitalWrite(MTR_R_FWD, LOW);
      digitalWrite(MTR_R_BACK, LOW);
    }
};

void petting(){
  Serial.println("DEBUG:  Petting");
  displayEmotion("lovely");
  moveWheels(0);
  moveWheels(3);
}

void stopCar(){
    delay(500);
    digitalWrite(MTR_L_FWD, LOW);
    digitalWrite(MTR_L_BACK, LOW);
    digitalWrite(MTR_R_FWD, LOW);
    digitalWrite(MTR_R_BACK, LOW);
}

void handleRemoteCommand(int command) {
    switch (command) {
        case IR_POWER_OFF:
            Serial.println("DEBUG: Power off");
            setColors(0, 0, 0);
            dimFactor = 0;
            break;

        case IR_BUTTON_0:
            Serial.println("DEBUG: On");
            setColors(255, 255, 255);
            dimFactor = 1;
            break;

        case IR_BUTTON_1:
            Serial.println("DEBUG: One RED");
            setColors(255, 0, 0);
            break;

        case IR_BUTTON_2:
            Serial.println("DEBUG: Two READING LIGHT");
            setColors(128, 32, 0);
            break;

        case IR_BUTTON_3:
            Serial.println("DEBUG: Three RELAX LIGHT");
            setColors(26, 6, 0);
            dimFactor = 0.5;
            break;

        case IR_ARROW_DOWN:
            Serial.println("DEBUG: Decrease dim");
            adjustDimFactor(0.75);
            break;

        case IR_ARROW_UP:
            Serial.println("DEBUG: Increase dim");
            adjustDimFactor(1.3);
            break;

        case IR_BUTTON_4:
            Serial.println("DEBUG: Happy Emotion");
            displayEmotion("happy");
            break;

        case IR_BUTTON_5:
            Serial.println("DEBUG: Sad Emotion");
            displayEmotion("sad");
            break;

        case IR_BUTTON_6:
            Serial.println("DEBUG: Lovely Emotion");
            displayEmotion("lovely");
            break;
      
      	case IR_VOL_UP:
          Serial.println("DEBUG: Moving Forward");
          moveWheels(0);  // Forward
          break;

      	case IR_BUTTON_PRE:
          Serial.println("DEBUG: Moving Left");
          moveWheels(1);  // Left
          break;

      	case IR_BUTTON_NEXT:
          Serial.println("DEBUG: Moving Right");
          moveWheels(2);  // Right
          break;

      	case IR_VOL_DOWN:
      		Serial.println("DEBUG: Moving Backward");
          moveWheels(3);  // Backward
          break;

      	case IR_BUTTON_PAUSE:
      		Serial.println("DEBUG: Stop");
          moveWheels(4);  // Stop
          break;

        case IR_BUTTON_EQ:
      		petting();
          break;

        default:
          Serial.print(F("DEBUG: Unknown command "));
      		Serial.println("0x" + String(command, HEX));
          break;
    }

    updateRGBLED();
}

void setColors(int r, int g, int b) {
    rBright = r;
    gBright = g;
    bBright = b;
}

void adjustDimFactor(float factor) {
    dimFactor *= factor;
    if (dimFactor > 1.0) {
        dimFactor = 1.0;
    } else if (dimFactor < 0.0) {
        dimFactor = 0.0;
    }
}

void updateRGBLED() {
    analogWrite(R_PIN, rBright * dimFactor);
    analogWrite(G_PIN, gBright * dimFactor);
    analogWrite(B_PIN, bBright * dimFactor);
}

void displayEmotion(String emotion) {
    lcd.clear();
    if (emotion == "happy") {
      lcd.setCursor(2, 1);
      lcd.print("^");
      lcd.setCursor(12, 1);
      lcd.print("^");
    } else if (emotion == "sad") {
      lcd.setCursor(2, 1);
      lcd.print("-");
      lcd.setCursor(12, 1);
      lcd.print("-");
    } else if (emotion == "lovely") {
      lcd.setCursor(2, 1);
      lcd.print("<3");
      lcd.setCursor(12, 1);
      lcd.print("<3");
    }
}