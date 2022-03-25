#pragma once

#include "variant.h"

#define PIN_SERIAL2_TX (10ul)              // TX on A10
#define PIN_SERIAL2_RX (9ul)               // RX on A9
#define PAD_SERIAL2_TX (UART_TX_PAD_2)
#define PAD_SERIAL2_RX (SERCOM_RX_PAD_1)

extern Uart Serial2;

void SERCOM0_Handler(void);
