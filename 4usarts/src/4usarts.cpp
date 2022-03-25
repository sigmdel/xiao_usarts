/*
 * xiao_4usarts
 *
 * Sample program showing how to set up four hardware serial interfaces
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
 * Serial4 uses the ALT-SERCOM1 module which is normally "reserved"
 *   for the SWD bus. Serial4 is defined in the ../lib/Serial4.h library.
 *   The pin assignment is
 *   TX is 17 (PA30) (= ALT-SERCOM1 Pad 2) and RX is 18 (= ALT-SERCOM1 Pad 3)
 *
 *   Note that A17 and A18 are not defined, hence the pins are identified
 *   by numbers (their index in the pin description table) and not by
 *   macros or number constants defined elsewhere.
 *
 *   This fourth hardware USART required the addition of two pin definitions
 *   at the end of the g_APinDescription[] table in the variant.cpp file
 *   (../framework-arduino-samd-seeed/variants/XIAO_m0/variant.cpp) as shown
 *   here
 *   |...
 *   |
 *   |
 *   | const PinDescription g_APinDescription[]=
 *   | {
 *   | ...
 *   | { PORTA, 24, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // USB/DM
 *   | { PORTA, 25, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE } // USB/DP
 *   |
 *   | // 17,18 - SWD Interface - added for 4th hardware USART
 *   | ,
 *   | { PORTA, 30, PIO_SERCOM_ALT, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, //SWCLK, TX: SERCOM1/PAD[2]
 *   | { PORTA, 31, PIO_SERCOM_ALT, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE } // SWDIO, RX: SERCOM1/PAD[3]
 *   |
 *   | } ;
 *   +----------------------------------------------
 *
 *
 * Wiring
 *
 *   Instead of looping each serial port TX to RX, feed each serial TX
 *   pin to the next serial port's RX pin in round-robin fashion:
 *
 *   Serial1-TX --> Serial2-RX             A6 --> A9
 *   Serial2-TX --> Serial3-RX            A10 --> A5
 *   Serial3-TX --> Serial4-RX             A4 --> SWDIO (PA31)
 *   Serial4-TX --> Serial1-TX   (PA30) SWCLK --> A7
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
 */

#include <Arduino.h>            // Needed for PlatformIO
#include "wiring_private.h"     // for pinPeripheral() function
#include "Serial2.h"
#include "Serial3.h"
#include "Serial4.h"

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
  Serial.println("\n\nxiao_4usarts");
  Serial.println("------------");

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

  // Serial4
  Serial.println("Setting up Serial4");
  Serial4.begin(USART_BAUD);
  pinPeripheral(PIN_SERIAL4_TX, PIO_SERCOM_ALT);  // not needed but will not break anything
  pinPeripheral(PIN_SERIAL4_RX, PIO_SERCOM_ALT);  // not needed but will not break anything


  Serial.println("Setup completed, starting loop");
  Serial.flush();
}

// delays between successive messages transmitted on each Serial device
#define SERIAL1_MESSAGE_INTERVAL 1000
#define SERIAL2_MESSAGE_INTERVAL 1300
#define SERIAL3_MESSAGE_INTERVAL 1600
#define SERIAL4_MESSAGE_INTERVAL 1900

// timers for these delays
unsigned long serial1Timer = millis();
unsigned long serial2Timer = serial1Timer;
unsigned long serial3Timer = serial1Timer;
unsigned long serial4Timer = serial1Timer;
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

  // Serial4
  //
  // Transmit every byte received from Serial3 to Serial = USBSerial
  while (Serial4.available()) {
    Serial.write(Serial4.read());
    Serial.flush();
  }

  if (millis() - serial4Timer >= SERIAL4_MESSAGE_INTERVAL) {
    Serial.printf("\nWriting %d to Serial4\n", runcount*4);
    Serial.flush();
    Serial4.printf("Serial4: %d\n", runcount*4);
    Serial4.flush();
    serial4Timer = millis();
  }

}
