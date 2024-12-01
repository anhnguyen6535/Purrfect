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
int IR_RECEIVE_PIN = 2;
int IR_SEND_PIN = 3;
int rPin = 10;
int gPin = 9;
int bPin = 6;

/*
* Remote vars
*/
const int IR_POWER_OFF = 0x45;
const int IR_BUTTON_0 = 0x16;
const int IR_BUTTON_1 = 0xC;
const int IR_BUTTON_2 = 0x18;
const int IR_BUTTON_3 = 0x5E;
const int IR_VOL_DOWN = 0x7;
const int IR_VOL_UP = 0x9;

/*
* Variables
*/
int rBright = 255;
int gBright = 255;
int bBright = 255;

float dimFactor = 1;



void setup() {
    // Start up Serial
    Serial.begin(115200);
    while (!Serial)

    // Setup LED
    pinMode(rPin, OUTPUT);    
    pinMode(gPin, OUTPUT);    
    pinMode(bPin, OUTPUT);    

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

        /*
         * Finally, check the received data and perform actions according to the received command
         */
        if (IrReceiver.decodedIRData.command == IR_POWER_OFF) {
          Serial.println("DEBUG: Power off");
          rBright = 0;
          gBright = 0;
          bBright = 0;
          dimFactor = 0;
        } 
        else if (IrReceiver.decodedIRData.command == IR_BUTTON_0) {
          Serial.println("DEBUG: On");
          rBright = 255;
          gBright = 255;
          bBright = 255;
          dimFactor = 1;
        } 
        else if (IrReceiver.decodedIRData.command == IR_BUTTON_1) {
          Serial.println("DEBUG: One RED");
          rBright = 255;
          gBright = 0;
          bBright = 0;

        } 
        else if (IrReceiver.decodedIRData.command == IR_BUTTON_2) {
          Serial.println("DEBUG: Two GREEN");
          rBright = 0;
          gBright = 255;
          bBright = 0;
        }
        else if (IrReceiver.decodedIRData.command == IR_BUTTON_3) {
          Serial.println("DEBUG: Three BLUE");
          rBright = 0;
          gBright = 0;
          bBright = 255;
        }
        else if (IrReceiver.decodedIRData.command == IR_VOL_DOWN) {
          Serial.println("DEBUG: Decrease dim");
          dimFactor *= .75;
        }
        else if (IrReceiver.decodedIRData.command == IR_VOL_UP) {
          Serial.println("DEBUG: Increase dim");
          dimFactor *= 1.3;
          if (dimFactor > 1){
            dimFactor = 1;
          } 
        }

        // Change LED colors
        analogWrite(rPin, rBright * dimFactor);
        analogWrite(gPin, gBright * dimFactor);
        analogWrite(bPin, bBright * dimFactor);
    }
}
