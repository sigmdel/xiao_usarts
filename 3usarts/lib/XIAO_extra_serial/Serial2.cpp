#include "Serial2.h"

Uart Serial2(&sercom0, PIN_SERIAL2_RX, PIN_SERIAL2_TX, PAD_SERIAL2_RX, PAD_SERIAL2_TX);

void SERCOM0_Handler(void) {
    Serial2.IrqHandler();
}
