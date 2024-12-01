/*
 * Based on the documentation on Adruino
 *
 */

#include <Arduino.h>

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


/*
* Define PINS
*/
const int IR_RECEIVE_PIN = 2;
const int IR_SEND_PIN = 3;
const int R_PIN = 10;
const int G_PIN = 9;
const int B_PIN = 6;

/*
* Remote vars
*/
const int IR_POWER_OFF = 0x45;
const int IR_BUTTON_0 = 0x16;
const int IR_BUTTON_1 = 0xC;
const int IR_BUTTON_2 = 0x18;
const int IR_BUTTON_3 = 0x5E;
const int IR_ARROW_DOWN = 0x7;
const int IR_ARROW_UP = 0x9;

/*
* Variables
*/
int rBright = 255;
int gBright = 255;
int bBright = 255;

float dimFactor = 1.0;



void setup() {
    // Start up Serial
    Serial.begin(115200);
    while (!Serial)

    // Setup LED
    pinMode(R_PIN, OUTPUT);
    pinMode(G_PIN, OUTPUT);
    pinMode(B_PIN, OUTPUT);

    // Info about program
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

    Serial.print(F("Ready to receive IR signals of protocols: "));
    printActiveIRProtocols(&Serial);
}

void loop() {
    /*
     * Check if received data is available and if yes, try to decode it.
     * Decoded result is in the IrReceiver.decodedIRData structure.
     *
     * E.g. command is in IrReceiver.decodedIRData.command
     * address is in command is in IrReceiver.decodedIRData.address
     * and up to 32 bit raw data in IrReceiver.decodedIRData.decodedRawData
     */
    if (IrReceiver.decode()) {

        /*
         * Print a summary of received data
         */
        // Unknown protocol
        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
            Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
            IrReceiver.printIRResultRawFormatted(&Serial, true);
            IrReceiver.resume(); 
        } else {
            IrReceiver.resume(); // Enable receiving of the next IR frame
            Serial.println(IrReceiver.lastDecodedCommand, HEX);
        }
        Serial.println();

        // Handle IR
        handleRemoteCommand(IrReceiver.decodedIRData.command);
    }
}

void handleRemoteCommand(int command){
    switch(command){
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

      default:
        Serial.println(F("DEBUG: Unknown command"));
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