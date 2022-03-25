#include "Serial3.h"

Uart Serial3(&sercom2, PIN_SERIAL3_RX, PIN_SERIAL3_TX, PAD_SERIAL3_RX, PAD_SERIAL3_TX);

void SERCOM2_Handler(void) {
    Serial3.IrqHandler();
}
