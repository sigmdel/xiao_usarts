#pragma once

#include "variant.h"

#define PIN_SERIAL4_TX (17ul)              // TX on SWCLK
#define PIN_SERIAL4_RX (18ul)              // RX on SWDIO
#define PAD_SERIAL4_TX (UART_TX_PAD_2)
#define PAD_SERIAL4_RX (SERCOM_RX_PAD_3)

extern Uart Serial4;

void SERCOM1_Handler(void);
