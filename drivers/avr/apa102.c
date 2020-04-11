/*
 * APA102 SPI Driver
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "apa102.h"
#include "pincontrol.h"
#include "../../lib/lufa/LUFA/Drivers/Peripheral/SPI.h"

void apa102_send_array(LED_TYPE *data, uint16_t leds) {  // Data is struct of 3 bytes. RGB - leds is number of leds in data
    SPI_Init(SPI_ORDER_MSB_FIRST | SPI_SCK_LEAD_FALLING |
            SPI_SAMPLE_TRAILING | SPI_MODE_MASTER); 

    // header
    SPI_SendByte(0);
    SPI_SendByte(0);
    SPI_SendByte(0);
    SPI_SendByte(0);

    uint16_t i = 0;
    for (i = 0; i < leds; i++) {
        SPI_SendByte(0b11101001); // low brightness for now
        SPI_SendByte(data[i].b);
        SPI_SendByte(data[i].g);
        SPI_SendByte(data[i].r);
    }

    // end sequence
    SPI_SendByte(0);
    SPI_SendByte(0);
    SPI_SendByte(0);
    SPI_SendByte(0);

    // end frame count from:
    // https://github.com/cpldcpu/light_ws2812/blob/master/light_apa102_AVR/Light_apa102/light_apa102.c
    for (i = 0; i < leds; i += 16) {
        SPI_SendByte(0);
    } 
}

// DI and CLK unused (future work: modify so there is an SPI and non SPI mode)
void inline apa102_setleds_pin(LED_TYPE *ledarray, uint16_t leds, uint8_t pinmask_DI, uint8_t pinmask_CLK) {
    SPI_Init(SPI_ORDER_MSB_FIRST | SPI_SCK_LEAD_FALLING |
            SPI_SAMPLE_TRAILING | SPI_MODE_MASTER); 

    apa102_send_array(ledarray, leds);
}

// Setleds for standard RGB
void inline apa102_setleds(LED_TYPE *ledarray, uint16_t leds) { apa102_setleds_pin(ledarray, leds, _BV(RGB_DI_PIN & 0xF), _BV(RGB_CLK_PIN & 0xF)); }