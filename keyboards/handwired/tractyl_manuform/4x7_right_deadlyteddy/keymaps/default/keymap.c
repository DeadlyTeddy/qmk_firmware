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

enum custom_layers {
    _COLEMAK_DH,
    _QWERTY,
    _LOWER,
    _ARROWS,
    _SYMBOLS,
    _PUNCT,
    _CLICK,
    _MEDIA,
};

// Home row layer taps
#define HOME_A LT(_ARROWS, KC_A)
#define HOME_R LT(_PUNCT, KC_R)
#define HOME_S LT(_LOWER, KC_S)
#define HOME_T LT(_SYMBOLS, KC_T)
#define HOME_D LT(_CLICK, KC_D)

const uint16_t PROGMEM combo_f5[] = {HOME_A, HOME_R, HOME_S, HOME_T, COMBO_END};
const uint16_t PROGMEM combo_gui_ctl_0[] = {KC_LSFT, HOME_S, HOME_T, COMBO_END};
const uint16_t PROGMEM combo_boot[] = {KC_W, KC_F, KC_P, KC_B, COMBO_END};

enum combo_names {
    COMBO_F5,
    COMBO_GUI_CTL_0,
    COMBO_BOOT,
    COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;

combo_t key_combos[] = {
    [COMBO_F5]        = COMBO(combo_f5, KC_F5),
    [COMBO_GUI_CTL_0] = COMBO(combo_gui_ctl_0, LGUI(LCTL(KC_0))),
    [COMBO_BOOT]      = COMBO(combo_boot, QK_BOOT),
};

bool get_combo_must_hold(uint16_t index, combo_t *combo) {
    return false;
}

// Short aliases
#define CAPSWRD QK_CAPS_WORD_TOGGLE
#define HOME_N LCTL_T(KC_N)
#define HOME_SLSH LT(_MEDIA, KC_SLSH)

// Custom tap/hold keycodes — tap outputs immediately on release, hold after TAPPING_TERM
enum custom_keycodes {
    TH_MINS_ENT = SAFE_RANGE,
    TH_LCLK_RCLK,
    TH_LOCK_CAD,
    TH_M_END,
    TH_G_HOME,
    DT_LSFT_SPC,
};

typedef struct {
    uint16_t tap_kc;
    uint16_t hold_kc;
    uint16_t timer;
    bool held;
} tap_hold_t;

typedef struct {
    uint16_t keycode;
    uint16_t timer;
    uint8_t tap_count;
} double_tap_t;

static tap_hold_t tap_holds[] = {
    [TH_MINS_ENT - SAFE_RANGE]   = {KC_MINS, KC_ENT,  0, false},
    [TH_LCLK_RCLK - SAFE_RANGE]  = {MS_BTN1, MS_BTN2, 0, false},
    [TH_LOCK_CAD - SAFE_RANGE]   = {LALT(KC_L), LCTL(LALT(KC_DEL)), 0, false},
    [TH_M_END - SAFE_RANGE]      = {KC_M, KC_END, 0, false},
    [TH_G_HOME - SAFE_RANGE]     = {KC_G, KC_HOME, 0, false},
};

static double_tap_t double_tap_lsft = {DT_LSFT_SPC, 0, 0};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode >= TH_MINS_ENT && keycode <= TH_G_HOME) {
        tap_hold_t *th = &tap_holds[keycode - SAFE_RANGE];
        if (record->event.pressed) {
            th->timer = timer_read();
            th->held = false;
        } else {
            if (th->held) {
                unregister_code16(th->hold_kc);
            } else {
                tap_code16(th->tap_kc);
            }
            th->held = false;
            th->timer = 0;
        }
        return false;
    }

    // Double-tap shift for space
    if (keycode == KC_LSFT) {
        if (record->event.pressed) {
            if (double_tap_lsft.tap_count == 0) {
                double_tap_lsft.timer = timer_read();
            }
            double_tap_lsft.tap_count++;

            if (double_tap_lsft.tap_count == 2) {
                tap_code(KC_SPC);
                double_tap_lsft.tap_count = 0;
                return false;
            }
        }
        return true;
    }

    return true;
}

void matrix_scan_user(void) {
    for (int i = 0; i < sizeof(tap_holds) / sizeof(tap_holds[0]); i++) {
        tap_hold_t *th = &tap_holds[i];
        if (th->timer && !th->held && timer_elapsed(th->timer) > TAPPING_TERM) {
            th->held = true;
            register_code16(th->hold_kc);
        }
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_COLEMAK_DH] = LAYOUT_4x7_right(
    KC_ESC ,KC_DEL , KC_Q  , KC_W  , KC_F  , KC_P  , KC_B  ,                                 KC_J  , KC_L  , KC_U  , KC_Y  ,KC_SCLN,KC_MINS,TH_LOCK_CAD,
    KC_F18 ,KC_BSPC, HOME_A, HOME_R, HOME_S, HOME_T, TH_G_HOME,                              TH_M_END, HOME_N, KC_E  , KC_I  , KC_O  ,KC_QUOT,KC_F22 ,
    KC_F19 ,KC_LCTL, KC_Z  , KC_X  , KC_C  , HOME_D, KC_V  ,                                 KC_K  , KC_H  ,KC_COMM, KC_DOT,HOME_SLSH,KC_GRV,KC_F23 ,
    KC_F20 ,KC_F21 ,KC_LWIN,KC_LALT,CAPSWRD,                                                                KC_MINS, KC_PLUS,KC_BSLS,DF(_QWERTY),KC_F24 ,
                                            KC_LSFT,KC_LCTL,                                        KC_SPC,
                                                    KC_LALT, KC_TAB,                   TH_MINS_ENT,
                                                    KC_BSPC,KC_LWIN,          KC_F17, KC_ENT
  ),

  [_QWERTY] = LAYOUT_4x7_right(
    KC_ESC ,KC_DEL , KC_Q  , KC_W  , KC_E  , KC_R  , KC_T  ,                                 KC_Y  , KC_U  , KC_I  , KC_O  , KC_P  ,KC_MINS,TH_LOCK_CAD,
    KC_F18 ,KC_BSPC, KC_A  , KC_S  , KC_D  , KC_F  , KC_G  ,                                 KC_H  , KC_J  , KC_K  , KC_L  ,KC_SCLN,KC_QUOT,KC_F22 ,
    KC_F19 ,KC_LCTL, KC_Z  , KC_X  , KC_C  , KC_V  , KC_B  ,                                 KC_N  , KC_M  ,KC_COMM, KC_DOT,KC_SLSH, KC_GRV,KC_F23 ,
    KC_F20 ,KC_F21 ,KC_LWIN,KC_LALT,CAPSWRD,                                                                KC_MINS, KC_PLUS,KC_BSLS,DF(_COLEMAK_DH),KC_F24 ,
                                            KC_SPC ,KC_LCTL,                                        KC_SPC,
                                                    KC_LALT, KC_TAB,                   TH_MINS_ENT,
                                                    KC_BSPC,KC_LWIN,          TH_LCLK_RCLK, KC_ENT
  ),

  [_LOWER] = LAYOUT_4x7_right( // Hold S
    _______,_______,_______,_______,_______,_______,KC_LBRC,                                XXXXXXX, KC_7  , KC_8  , KC_9  ,KC_SLSH,XXXXXXX,XXXXXXX,
    _______,_______,KC_HOME,KC_PGUP,KC_PGDN,KC_END ,KC_LPRN,                                XXXXXXX, KC_4  , KC_5  , KC_6  ,KC_ASTR,XXXXXXX,XXXXXXX,
    _______,_______,_______,_______,_______,_______,_______,                                XXXXXXX, KC_1  , KC_2  , KC_3  ,KC_EQL ,XXXXXXX,XXXXXXX,
    _______,_______,_______,_______,KC_PSCR,                                                                KC_MINS,KC_PLUS,XXXXXXX,XXXXXXX,XXXXXXX,
                                            _______,_______,                                KC_0 ,
                                                    _______,_______,                _______,
                                                    _______,_______,        _______,_______
  ),

  [_ARROWS] = LAYOUT_4x7_right( // Hold A
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,KC_LEFT,KC_DOWN, KC_UP ,KC_RGHT,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                                                XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
                                            XXXXXXX,XXXXXXX,                                XXXXXXX,
                                                    XXXXXXX,XXXXXXX,                XXXXXXX,
                                                    XXXXXXX,XXXXXXX,        XXXXXXX,XXXXXXX
  ),

  [_SYMBOLS] = LAYOUT_4x7_right( // Hold T
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,KC_TILD,KC_PIPE, KC_EQL,KC_PLUS,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,KC_LPRN,KC_RPRN,KC_LBRC,KC_RBRC,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,KC_LABK,KC_RABK,KC_LCBR,KC_RCBR,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                                                XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
                                            XXXXXXX,XXXXXXX,                                XXXXXXX,
                                                    XXXXXXX,XXXXXXX,                XXXXXXX,
                                                    XXXXXXX,XXXXXXX,        XXXXXXX,XXXXXXX
  ),

  [_PUNCT] = LAYOUT_4x7_right( // Hold R
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,LGUI(KC_LEFT),LGUI(KC_RGHT),XXXXXXX,                    XXXXXXX,KC_EXLM, KC_AT ,KC_HASH, KC_DLR,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,KC_PERC,KC_CIRC,KC_AMPR,KC_ASTR,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                                                XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
                                            XXXXXXX,XXXXXXX,                                XXXXXXX,
                                                    XXXXXXX,XXXXXXX,                XXXXXXX,
                                                    XXXXXXX,XXXXXXX,        XXXXXXX,XXXXXXX
  ),

  [_CLICK] = LAYOUT_4x7_right( // Hold D
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,MS_BTN1,MS_BTN2,MS_BTN3,XXXXXXX,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,MS_BTN4,MS_BTN5,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                                                XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
                                            XXXXXXX,XXXXXXX,                                XXXXXXX,
                                                    XXXXXXX,XXXXXXX,                XXXXXXX,
                                                    XXXXXXX,XXXXXXX,        XXXXXXX,XXXXXXX
  ),

  [_MEDIA] = LAYOUT_4x7_right( // Hold '/'
    XXXXXXX,XXXXXXX,XXXXXXX,KC_VOLD,KC_VOLU,KC_MUTE,XXXXXXX,                                XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,KC_MPRV,KC_MNXT,XXXXXXX,KC_MPLY,XXXXXXX,                                XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                                                XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
                                            XXXXXXX,XXXXXXX,                                XXXXXXX,
                                                    XXXXXXX,XXXXXXX,                XXXXXXX,
                                                    XXXXXXX,XXXXXXX,        XXXXXXX,XXXXXXX
  ),

};

// Matrix debugging - uncomment to see raw matrix positions
// void matrix_scan_user(void) {
//     static uint32_t last_print = 0;

//     if (timer_elapsed32(last_print) > 100) {
//         last_print = timer_read32();

//         for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
//             for (uint8_t col = 0; col < MATRIX_COLS; col++) {
//                 if (matrix_is_on(row, col)) {
//                     uprintf("KEY: [%d,%d]\n", row, col);
//                 }
//             }
//         }
//     }
// }
