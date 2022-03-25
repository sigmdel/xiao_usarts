#include "Serial4.h"

Uart Serial4(&sercom1, PIN_SERIAL4_RX, PIN_SERIAL4_TX, PAD_SERIAL4_RX, PAD_SERIAL4_TX);

void SERCOM1_Handler(void) {
    Serial4.IrqHandler();
}
