/* 
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
#include QMK_KEYBOARD_H
#include "APDS9960.h"

#define _BASE 0

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /*
        BASE LAYER
   /---------------------------------------`
   |   ESC   |    /    |    *    |    -    |
   |---------|---------|---------|---------|
   |    7    |    8    |    9    |    +    |
   |---------|---------|---------|---------|
   |    4    |    5    |    6    |    +    |
   |---------|---------|---------|---------|
   |    1    |    2    |    3    |   Tab   |
   |---------|---------|---------|---------|
   |    0    |    0    |    .    |  Enter  |
   \---------------------------------------'
  */
  [_BASE] = LAYOUT(
    KC_ESC,    KC_KP_SLASH,    KC_KP_ASTERISK,     KC_KP_MINUS, 
    KC_7,      KC_8,           KC_9,               KC_KP_PLUS, 
    KC_4,      KC_5,           KC_6,               KC_KP_PLUS, 
    KC_1,      KC_2,           KC_3,               KC_TAB, 
    KC_0,      KC_0,           KC_DOT,             KC_KP_ENTER
  ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // If console is enabled, it will print the matrix position and status of each key pressed
/*
#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: %u, col: %u, row: %u, pressed: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed);
#endif 
*/
  return true;
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  //debug_enable = true;
  //debug_matrix = true;
  //debug_keyboard = true;
  //debug_mouse = true;
}

void matrix_init_user(void) {
  rgblight_init();
  apds9960_init();
  
  wait_ms(1);
  setProximityGain(PGAIN_2X);
  wait_ms(1);
  enableProximitySensor(false);
  //enableLightSensor(false);
  //wait_ms(1);
  //enableGestureSensor(false);
  rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
  rgblight_sethsv_noeeprom(128, 70, 100);
}

//static uint16_t lastlight = 0;

void matrix_scan_user(void) {
  uint8_t data = 0;
  if ( readProximity(&data) ) {
    rgblight_sethsv_noeeprom(data, 255, 255);
  }
  
  /*
  uint16_t light = 0;
  if (readAmbientLight(&light)) {
    if (light > 255) {
      light = 255;
    }
    light = (light*1 + lastlight*99) / (100);
    rgblight_sethsv_noeeprom(255, 0, light);
    lastlight = light;
  }
  */
  /*
  if ( isGestureAvailable() ) {
    switch ( readGesture() ) {
      case DIR_UP:
        rgblight_sethsv_noeeprom(0, 70, 100);
        break;
      case DIR_DOWN:
        rgblight_sethsv_noeeprom(50, 70, 100);
        break;
      case DIR_LEFT:
        rgblight_sethsv_noeeprom(100, 70, 100);
        break;
      case DIR_RIGHT:
        rgblight_sethsv_noeeprom(150, 70, 100);
        break;
      case DIR_NEAR:
        rgblight_sethsv_noeeprom(0, 70, 100);
        break;
      case DIR_FAR:
        rgblight_sethsv_noeeprom(0, 70, 100);
        break;
      default:
        //rgblight_sethsv_noeeprom(0, 0, 0);
        break;
    } 
  }
  */
  
}

void led_set_user(uint8_t usb_led) {

}

void encoder_update_user(uint8_t index, bool clockwise) {
  /*  Custom encoder control - handles CW/CCW turning of encoder
   *  Default behavior:
   *      Audio volume
   */
   
  if (index == 0) {
    switch (biton32(layer_state)) {
      default:
        if (clockwise) {
          tap_code(KC_AUDIO_VOL_DOWN);
        } else {
          tap_code(KC_AUDIO_VOL_UP);
        }
        break;
    }
  }
}

void dip_switch_update_user(uint8_t index, bool active) { 
  switch (index) {
    case 0:
      if(active) {
        tap_code(KC_MUTE);
      }
      break;

    default:
      break;
  }
}
