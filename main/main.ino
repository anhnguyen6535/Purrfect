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
const int IR_RECEIVE_PIN = 2;
const int IR_SEND_PIN = 3;
const int R_PIN = 10;
const int G_PIN = 9;
const int B_PIN = 6;
const int TRIG_PIN = 5;
const int ECHO_PIN = 4;

const int LCD_rs = 12;
const int LCD_en = 11;
const int LCD_d4 = 13;
const int LCD_d5 = 8;
const int LCD_d6 = 7;
const int LCD_d7 = 3;

/*
 * Remote vars
 */
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

    // Info about program
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    // Start the receiver
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

    // Start the LCD
    lcd.begin(16,2);
  	delay(100);
  	lcd.print("Cat!");

    Serial.print(F("Ready to receive IR signals of protocols: "));
    printActiveIRProtocols(&Serial);
}

void loop() {
    // Handle IR Receiver
    if (IrReceiver.decode()) {
        // Handle IR
        handleRemoteCommand(IrReceiver.decodedIRData.command);

        IrReceiver.resume(); // Enable receiving of the next IR frame
    }

    // Handle Ultrasonic Sensor
    float distance = pollUltrasonicSensor();
    if (distance < 10) {
        // purr???
        Serial.println("Purr");
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
      lcd.setCursor(4, 1);
      lcd.print("^");
      lcd.setCursor(12, 1);
      lcd.print("^");
    } else if (emotion == "sad") {
      lcd.setCursor(4, 1);
      lcd.print("-");
      lcd.setCursor(12, 1);
      lcd.print("-");
    } else if (emotion == "lovely") {
      lcd.setCursor(4, 1);
      lcd.print("<3");
      lcd.setCursor(12, 1);
      lcd.print("<3");
    }
}
