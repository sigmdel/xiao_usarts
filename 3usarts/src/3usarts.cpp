/*
 * 3usarts
 *
 * Sample program showing how to set up three hardware serial interfaces
 * on the Seeeduino XIAO board based on the Sam D21G microcontroller
 *
 */

// Copyright 2022, Michel Deslierres, no rights reserved.
// In those jurisdictions where releasing a work into the public domain may be a problem,
// the BSD Zero Clause License <https://spdx.org/licenses/0BSD.html> applies.
// SPDX-License-Identifier: 0BSD

/*
 * Serial devices (USARTs - Universal synchronous and asynchronous receiver-transmitters)
 *   While the SAM D21/D51 support synchronous serial interfaces, only asynchronous
 *   serial communication is used here. In other words, the serial interfaces are
 *   instances of the Uart class.
 *
 * Serial1 is the default ALT-SERCOM4 module with the default pins:
 *   TX is A6 (= ALT-SERCOM4 Pad 0) and RX is A7 (= ALT-SERCOM4 Pad 1)
 *
 * Serial2 uses the ALT-SERCOM0 module which is normally "reserved"
 *   for the SPI serial connection. Serial2 is defined in the
 *   ../lib/Serial2.h library. The pin assignment is
 *   TX is A10 (= ALT-SERCOM0 Pad 2) and RX is A9 (= ALT-SERCOM0 Pad 1)
 *
 * Serial3 uses the ALT-SERCOM2 module which is normally "reserved"
 *   for the I²C serial connection. Serial3 is defined in the
 *   ../lib/Serial3.h library. The pin assignment is
 *   TX is A4 (= ALT-SERCOM2 Pad 0) and RX is A5 (= ALT-SERCOM2 Pad 1)
 *
 * Serial3Alt (which needs to be created) will use ALT-SERCOM2 which is
 * "reserved" for the I²C interface. It is defined in the
 *  ../lib/Serial3Alt.h library. The pin assignment is
 *  TX is A2 (= ALT-SERCOM2 Pad 2) and RX is A3 (= ALT-SERCOM2 Pad 3)
 *
 *
 * Wiring
 *
 *  Instead of looping each serial port TX to RX, feed each serial TX
 *  pin to the next serial port's RX pin in round-robin fashion.
 *
 * If Serial2 and Serial3 are used the wiring is
 *
 *    Serial1-TX --> Serial2-RX   A6 --> A9
 *    Serial2-TX --> Serial3-RX  A10 --> A5
 *    Serial3-TX --> Serial1-RX   A4 --> A7
 *
 * If Serial2 and Serial3Alt are used the wiring is
 *    Serial1-TX --> Serial2-RX   A6 --> A9
 *    Serial2-TX --> Serial3-RX  A10 --> A3
 *    Serial3-TX --> Serial1-RX   A2 --> A7
 *
 * The USE_ALT_SERIAL3 macro selects Serial3Alt
 *
 *
 * References
 *
 *   Three, Nay Four Hardware Serial Ports on a SAM D21 XIAO (2022/03/23) by Michel Deslierres
 *     @ https://sigmdel.ca/michel/ha/xiao/seeeduino_xiao_3usarts_en.html
 *
 *   XIAO Serial Extension 2 (2021/03/03) by kio denshi
 *     @ http://kio-denshi.com/hp/index.php?Technical%20Information%2Fetc.%2FXIAO%20Serial%20Extension%202
 *
 *   Seeeduino XIAO Serial Communication Interfaces (SERCOM) (2020/05/04-2022/03/23) by Michel Deslierres
 *     @ https://sigmdel.ca/michel/ha/xiao/seeeduino_xiao_sercom_en.html
 *
 * March 23, 2022, Michel Deslierres
 * 
 */
 
//#define USE_ALT_SERIAL3

#include <Arduino.h>            // Needed for PlatformIO
#include "wiring_private.h"     // for pinPeripheral() function
#include "Serial2.h"
#ifndef USE_ALT_SERIAL3
  #include "Serial3.h"
#else
  #include "Serial3Alt.h"
  #define Serial3 Serial3Alt     // a trick to make the rest independant of the choice
#endif

#define USART_BAUD    115200    // Baud for USARTs

void setup() {
  // Wait up to 10 seconds for Serial (= USBSerial) port to come up.
  // Usual wait is 0.5 second.
  unsigned long startserial = millis();
  while (!Serial && (millis() - startserial < 10000)) ;
  // Serial is now up.

  Serial.println("8 second initial delay");
  // Time to start the IDE serial monitor or to upload new firmware
  for (int i=8; i>-1; i--) {
    Serial.printf("\rStartup delay: %d", i);
    delay(1000);
  }

  // Greeting as we start
  Serial.println("\n\n3usarts");
  Serial.println("-------");

  // Serial1
  Serial.println("Setting up Serial1");
  Serial1.begin(USART_BAUD);

  // Serial2
  Serial.println("Setting up Serial2");
  Serial2.begin(USART_BAUD);
  pinPeripheral(PIN_SERIAL2_TX, PIO_SERCOM_ALT);  // may not be needed but will not break anything
  pinPeripheral(PIN_SERIAL2_RX, PIO_SERCOM_ALT);  // may not be needed but will not break anything

  // Serial3
  Serial.println("Setting up Serial3");
  Serial3.begin(USART_BAUD);
  pinPeripheral(PIN_SERIAL3_TX, PIO_SERCOM_ALT);  // may not be needed but will not break anything
  pinPeripheral(PIN_SERIAL3_RX, PIO_SERCOM_ALT);  // may not be needed but will not break anything

  Serial.println("Setup completed, starting loop");
  Serial.flush();
}

// delays between successive messages transmitted on each Serial device
#define SERIAL1_MESSAGE_INTERVAL 1000
#define SERIAL2_MESSAGE_INTERVAL 1300
#define SERIAL3_MESSAGE_INTERVAL 1600

// timers for these delays
unsigned long serial1Timer = millis();
unsigned long serial2Timer = serial1Timer;
unsigned long serial3Timer = serial1Timer;
int runcount = 0;



void loop(){

  // Serial1
  //
  // Transmit every byte received from Serial1 to Serial = USBSerial
  while (Serial1.available()) {
    Serial.write(Serial1.read());
    Serial.flush();
  }

  if (millis() - serial1Timer >= SERIAL1_MESSAGE_INTERVAL) {
    runcount++;
    Serial.printf("\nWriting runcount %d to Serial1\n", runcount);
    Serial.flush();
    Serial1.printf("Serial1: %d\n", runcount);
    Serial1.flush();
    serial1Timer = millis();
  }

  // Serial2
  //
  // Transmit every byte received from Serial2 to Serial = USBSerial
  while (Serial2.available()) {
    Serial.write(Serial2.read());
    Serial.flush();
  }

  if (millis() - serial2Timer >= SERIAL2_MESSAGE_INTERVAL) {
    Serial.printf("\nWriting %d to Serial2\n", runcount*2);
    Serial.flush();
    Serial2.printf("Serial2: %d\n", runcount*2);
    Serial2.flush();
    serial2Timer = millis();
  }

  // Serial3
  //
  // Transmit every byte received from Serial3 to Serial = USBSerial
  while (Serial3.available()) {
    Serial.write(Serial3.read());
    Serial.flush();
  }

  if (millis() - serial3Timer >= SERIAL3_MESSAGE_INTERVAL) {
    Serial.printf("\nWriting %d to Serial3\n", runcount*3);
    Serial.flush();
    Serial3.printf("Serial3: %d\n", runcount*3);
    Serial3.flush();
    serial3Timer = millis();
  }

}
