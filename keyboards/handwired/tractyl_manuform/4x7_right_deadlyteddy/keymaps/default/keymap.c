/* Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
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

const uint16_t PROGMEM combo1[] = {KC_A, KC_R, KC_S, KC_T, COMBO_END};
const uint16_t PROGMEM combo2[] = {KC_LSFT, KC_S, KC_T, COMBO_END};
const uint16_t PROGMEM bootloader_combo1[] = {KC_1, KC_2, KC_3, COMBO_END};
const uint16_t PROGMEM bootloader_combo2[] = {KC_B, KC_C, KC_D, COMBO_END};
combo_t key_combos[] = {
    COMBO(combo1, KC_F5),
    COMBO(combo2, LGUI(LCTL(KC_0))),
    COMBO(bootloader_combo1, QK_BOOT),
    COMBO(bootloader_combo2, QK_BOOT),
};
uint16_t COMBO_LEN = sizeof(key_combos) / sizeof(key_combos[0]);

enum custom_layers {
    _COLEMAK_DH,
    _LOWER,
    _RAISE,
};


#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_COLEMAK_DH] = LAYOUT_4x7_right(
    KC_1  , KC_2  , KC_3  , KC_4  , KC_5  , KC_6  , KC_7  ,                                 KC_8  , KC_9  , KC_0  , KC_A  , KC_B  , KC_C  , KC_D  ,
    KC_E  , KC_F  , KC_G  , KC_H  , KC_I  , KC_J  , KC_K  ,                                 KC_L  , KC_M  , KC_N  , KC_O  , KC_P  , KC_Q  , KC_R  ,
    KC_S  , KC_T  , KC_U  , KC_V  , KC_W  , KC_X  , KC_Y  ,                                 KC_Z  , KC_MINS, KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS, KC_GRV,
    KC_SCLN, KC_QUOT, KC_COMM, KC_DOT, KC_SLSH,                                                          KC_1  , KC_2  , KC_3  , KC_4  , KC_5  ,
                                            KC_6  , KC_7  ,                                         KC_8  ,
                                                    KC_9  , KC_0  ,                   KC_MINS,
                                                    KC_EQL, KC_LBRC,          KC_RBRC, KC_BSLS
  ),

  [_LOWER] = LAYOUT_4x7_right(
    _______,_______,_______,_______,_______,_______,KC_LBRC,                                KC_RBRC, KC_P7 , KC_P8 , KC_P9 ,_______,KC_PLUS,_______,
    _______,_______,KC_HOME,KC_PGUP,KC_PGDN,KC_END ,KC_LPRN,                                KC_RPRN, KC_P4 , KC_P5 , KC_P6 ,KC_MINS,KC_PIPE,_______,
    _______,_______,_______,_______,_______,_______,_______,                                _______, KC_P1 , KC_P2 , KC_P3 ,KC_EQL ,KC_UNDS,_______,
    _______,_______,_______,_______,KC_PSCR,                                                                _______,_______,_______,_______,_______,
                                            _______,_______,                                KC_P0 ,
                                                    _______,_______,                _______,
                                                    _______,_______,        _______,_______
  ),

  [_RAISE] = LAYOUT_4x7_right(
    _______,_______,_______,_______,_______,_______,KC_LBRC,                                KC_RBRC,_______,KC_NUM,KC_INS ,KC_SCRL, KC_MUTE,_______,
    _______,_______,KC_LEFT,KC_UP  ,KC_DOWN,KC_RGHT,KC_LPRN,                                KC_RPRN,KC_MPRV,KC_MPLY,KC_MNXT,_______,KC_VOLU,_______,
    _______,_______,_______,_______,_______,_______,_______,                                _______,_______,_______,_______,_______,KC_VOLD,_______,
    _______,_______,_______,_______,_______,                                                                _______,_______,_______,_______,_______,
                                            _______,_______,                                KC_EQL ,
                                                    _______,_______,                _______,
                                                    _______,_______,        _______,_______
  ),
};
