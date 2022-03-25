#pragma once

#include "variant.h"

#define PIN_SERIAL3_TX (2ul)              // TX on A2
#define PIN_SERIAL3_RX (3ul)              // RX on A3
#define PAD_SERIAL3_TX (UART_TX_PAD_2)
#define PAD_SERIAL3_RX (SERCOM_RX_PAD_3)

extern Uart Serial3Alt;

void SERCOM2_Handler(void);
