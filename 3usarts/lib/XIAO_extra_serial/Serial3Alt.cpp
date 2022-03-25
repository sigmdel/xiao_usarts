#include "Serial3Alt.h"

Uart Serial3Alt(&sercom2, PIN_SERIAL3_RX, PIN_SERIAL3_TX, PAD_SERIAL3_RX, PAD_SERIAL3_TX);

void SERCOM2_Handler(void) {
    Serial3Alt.IrqHandler();
}
