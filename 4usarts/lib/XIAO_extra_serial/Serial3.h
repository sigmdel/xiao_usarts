#pragma once

#include "variant.h"

#define PIN_SERIAL3_TX (4ul)              // TX on A4
#define PIN_SERIAL3_RX (5ul)              // RX on A5
#define PAD_SERIAL3_TX (UART_TX_PAD_0)
#define PAD_SERIAL3_RX (SERCOM_RX_PAD_1)

extern Uart Serial3;

void SERCOM2_Handler(void);
