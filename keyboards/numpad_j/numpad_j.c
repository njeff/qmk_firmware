/*
2019

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "numpad_j.h"
#include "..\..\lib\lufa\LUFA\Drivers\Peripheral\SPI.h"
#include <print.h>


void keyboard_pre_init_kb(void) {
  // Set the layer LED IO as outputs
  setPinOutput(LAYER_INDICATOR_LED_0);
  
  keyboard_pre_init_user();
}

void shutdown_user() {
  // Shutdown the layer LEDs
  writePinLow(LAYER_INDICATOR_LED_0);
}

layer_state_t layer_state_set_kb(layer_state_t state) {
  // Layer LEDs act as binary indication of current layer
  uint8_t layer = biton32(state);
  writePin(LAYER_INDICATOR_LED_0, layer & 0b1);
  return layer_state_set_user(state);
}

// Optional override functions below.
// You can leave any or all of these undefined.
// These are only required if you want to perform custom actions.

void matrix_init_kb(void) {
  // put your keyboard start-up code here
  // runs once when the firmware starts up
  for (int i = 0; i < 2; i++) {
    writePin(LAYER_INDICATOR_LED_0, true);
    wait_ms(100);
    writePin(LAYER_INDICATOR_LED_0, false);
    wait_ms(100);
  }
  
  matrix_init_user();

  // Set SS to output and drive it high (not connected to anything)
  // try to keep ATMega in master mode.
  // PORTB |= (1 << 0);
  // DDRB |= (1 << 0);

  // Aside: Figure out why the ATMega doesn't stay in master mode.
  // I suspect that the keyboard matrix scan initializing is somehow resetting this
  // (Setting SS to input low)
  // SPI_Init here doesn't do anything. But doing it in the matrix scan does work.
  // I only need to do it once in matrix_scan_kb too.
}

void matrix_scan_kb(void) {
  // put your looping keyboard code here
  // runs every cycle (a lot)
  matrix_scan_user();

  // LED tests
  SPI_Init(SPI_ORDER_MSB_FIRST | SPI_SCK_LEAD_FALLING |
          SPI_SAMPLE_TRAILING | SPI_MODE_MASTER); 

  SPI_SendByte(0);
  for (int i = 0; i < 9; i++) {
    for (int i = 0; i < 3; i++) {
      SPI_SendByte(0xFF);
    }
    for (int i = 0; i < 1; i++) {
      SPI_SendByte(0x00);
    }
  }
  SPI_SendByte(0xFF);
  for (uint16_t i = 0; i < 5 + 9 / 16; i++) {
    SPI_SendByte(0);
  }
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
  // put your per-action keyboard code here
  // runs for every action, just before processing by the firmware

  return process_record_user(keycode, record);
}

void led_set_kb(uint8_t usb_led) {
  // put your keyboard LED indicator (ex: Caps Lock LED) toggling code here

  led_set_user(usb_led);
}
