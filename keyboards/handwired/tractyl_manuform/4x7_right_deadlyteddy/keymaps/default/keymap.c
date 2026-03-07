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
    _NUMS,
    _ARROWS,
    _SYMBOLS,
    _PUNCT,
    _CLICK,
    _MEDIA,
    _QWERTY,
};

// Home row layer taps
#define HOME_A LT(_ARROWS, KC_A)
#define HOME_R LT(_PUNCT, KC_R)
#define HOME_S LT(_NUMS, KC_S)
#define HOME_T LT(_SYMBOLS, KC_T)
#define HOME_D LT(_CLICK, KC_D)

const uint16_t PROGMEM combo_f5[] = {HOME_A, HOME_R, HOME_S, HOME_T, COMBO_END};
const uint16_t PROGMEM combo_gui_ctl_0[] = {KC_LSFT, HOME_S, HOME_T, COMBO_END};
const uint16_t PROGMEM combo_boot[] = {KC_W, KC_F, KC_P, KC_B, COMBO_END};
const uint16_t PROGMEM combo_esc[] = {KC_W, KC_F, COMBO_END};

enum combo_names {
    COMBO_F5,
    COMBO_GUI_CTL_0,
    COMBO_BOOT,
    COMBO_ESC,
    COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;

combo_t key_combos[] = {
    [COMBO_F5]        = COMBO(combo_f5, KC_F5),
    [COMBO_GUI_CTL_0] = COMBO(combo_gui_ctl_0, LGUI(LCTL(KC_0))),
    [COMBO_BOOT]      = COMBO(combo_boot, QK_BOOT),
    [COMBO_ESC]       = COMBO(combo_esc, KC_ESC),
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
    Z_ARROWS_SHIFTCTRL,
};

typedef struct {
    uint16_t tap_kc;
    uint16_t hold_kc;
    uint16_t timer;
    bool held;
} tap_hold_t;

static tap_hold_t tap_holds[] = {
    [TH_MINS_ENT - SAFE_RANGE]   = {KC_MINS, KC_ENT,  0, false},
    [TH_LCLK_RCLK - SAFE_RANGE]  = {MS_BTN1, MS_BTN2, 0, false},
    [TH_LOCK_CAD - SAFE_RANGE]   = {LALT(KC_L), LCTL(LALT(KC_DEL)), 0, false},
};

static struct {
    uint16_t timer;
    bool held;
} z_state = {0, false};
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Shift + number on _NUMS = F-key
    if (record->event.pressed && IS_LAYER_ON(_NUMS) && (get_mods() & MOD_MASK_SHIFT)) {
        uint16_t fkey = 0;
        switch (keycode) {
            case KC_1: fkey = KC_F1;  break; case KC_2: fkey = KC_F2;  break;
            case KC_3: fkey = KC_F3;  break; case KC_4: fkey = KC_F4;  break;
            case KC_5: fkey = KC_F5;  break; case KC_6: fkey = KC_F6;  break;
            case KC_7: fkey = KC_F7;  break; case KC_8: fkey = KC_F8;  break;
            case KC_9: fkey = KC_F9;  break; case KC_0: fkey = KC_F10; break;
            case KC_MINS: fkey = KC_F11; break; case KC_EQL: fkey = KC_F12; break;
        }
        if (fkey) {
            uint8_t mods = get_mods();
            del_mods(MOD_MASK_SHIFT);
            tap_code(fkey);
            set_mods(mods);
            return false;
        }
    }

    if (keycode >= TH_MINS_ENT && keycode <= TH_LOCK_CAD) {
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

    if (keycode == Z_ARROWS_SHIFTCTRL) {
        uint8_t mods = get_mods() | get_oneshot_mods() | get_weak_mods();
        if (mods) {
            if (record->event.pressed) {
                register_code(KC_Z);
            } else {
                unregister_code(KC_Z);
            }
            return false;
        }

        if (record->event.pressed) {
            z_state.timer = timer_read();
            z_state.held = false;
        } else {
            if (!z_state.held) {
                tap_code(KC_Z);
            }
            layer_off(_ARROWS);
            unregister_code(KC_LSFT);
            unregister_code(KC_LCTL);
            z_state.held = false;
            z_state.timer = 0;
        }
        return false;
    }

    return true;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HOME_A:
        case HOME_S:
            return 200;
        default:
            return TAPPING_TERM;
    }
}

void matrix_scan_user(void) {
    for (int i = 0; i < sizeof(tap_holds) / sizeof(tap_holds[0]); i++) {
        tap_hold_t *th = &tap_holds[i];
        if (th->timer && !th->held && timer_elapsed(th->timer) > TAPPING_TERM) {
            th->held = true;
            register_code16(th->hold_kc);
        }
    }

    if (z_state.timer && !z_state.held && timer_elapsed(z_state.timer) > TAPPING_TERM) {
        z_state.held = true;
        layer_on(_ARROWS);
        register_code(KC_LSFT);
        register_code(KC_LCTL);
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_COLEMAK_DH] = LAYOUT_4x7_right(
    KC_ESC ,KC_DEL , KC_Q  , KC_W  , KC_F  , KC_P  , KC_B  ,                                 KC_J  , KC_L  , KC_U  , KC_Y  ,KC_SCLN,KC_MINS,TH_LOCK_CAD,
    KC_F18 ,KC_BSPC, HOME_A, HOME_R, HOME_S, HOME_T, KC_G  ,                                 KC_M  , HOME_N, KC_E  , KC_I  , KC_O  ,KC_QUOT,KC_F22 ,
    KC_F19 ,KC_LCTL, Z_ARROWS_SHIFTCTRL, KC_X  , KC_C  , HOME_D, KC_V  ,                                 KC_K  , KC_H  ,KC_COMM, KC_DOT,HOME_SLSH,KC_GRV,KC_F23 ,
    KC_F20 ,KC_F21 ,KC_LWIN,KC_LALT,CAPSWRD,                                                                KC_MINS, KC_EQL ,KC_BSLS,DF(_QWERTY),KC_F24 ,
                                            KC_LSFT,KC_LCTL,                                        KC_SPC,
                                                    KC_LALT, KC_TAB,                   TH_MINS_ENT,
                                                    KC_BSPC,KC_LWIN,          KC_F17, KC_ENT
  ),

  [_QWERTY] = LAYOUT_4x7_right(
    KC_ESC ,KC_DEL , KC_Q  , KC_W  , KC_E  , KC_R  , KC_T  ,                                 KC_Y  , KC_U  , KC_I  , KC_O  , KC_P  ,KC_MINS,TH_LOCK_CAD,
    KC_F18 ,KC_BSPC, KC_A  , KC_S  , KC_D  , KC_F  , KC_G  ,                                 KC_H  , KC_J  , KC_K  , KC_L  ,KC_SCLN,KC_QUOT,KC_F22 ,
    KC_F19 ,KC_LCTL, KC_Z  , KC_X  , KC_C  , KC_V  , KC_B  ,                                 KC_N  , KC_M  ,KC_COMM, KC_DOT,KC_SLSH, KC_GRV,KC_F23 ,
    KC_F20 ,KC_F21 ,KC_LWIN,KC_LALT, KC_M  ,                                                                KC_MINS, KC_EQL ,KC_BSLS,DF(_COLEMAK_DH),KC_F24 ,
                                            KC_SPC ,KC_LCTL,                                        KC_SPC,
                                                    KC_LALT, KC_TAB,                   TH_MINS_ENT,
                                                    KC_BSPC,KC_LWIN,          TH_LCLK_RCLK, KC_ENT
  ),

  [_NUMS] = LAYOUT_4x7_right( // Hold S
    _______,_______,_______,_______,_______,_______,KC_LBRC,                                XXXXXXX, KC_7  , KC_8  , KC_9  ,KC_SLSH,XXXXXXX,XXXXXXX,
    _______,_______,KC_HOME,_______,KC_PGDN,KC_END ,KC_LPRN,                                XXXXXXX, KC_4  , KC_5  , KC_6  ,KC_ASTR,XXXXXXX,XXXXXXX,
    _______,_______,_______,_______,_______,_______,_______,                                XXXXXXX, KC_1  , KC_2  , KC_3  ,KC_EQL ,XXXXXXX,XXXXXXX,
    _______,_______,_______,_______,KC_PSCR,                                                                KC_MINS,KC_PLUS,XXXXXXX,XXXXXXX,XXXXXXX,
                                            _______,_______,                                KC_0 ,
                                                    _______,_______,                _______,
                                                    _______,_______,        _______,_______
  ),

  [_ARROWS] = LAYOUT_4x7_right( // Hold A
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,XXXXXXX,KC_PGDN,KC_PGUP,XXXXXXX,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                KC_HOME,KC_LEFT,KC_DOWN, KC_UP ,KC_RGHT,KC_END ,XXXXXXX,
    XXXXXXX,XXXXXXX,_______,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                                                XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
                                            KC_LSFT,KC_LCTL,                                KC_LSFT,
                                                    XXXXXXX,XXXXXXX,                KC_LCTL,
                                                    XXXXXXX,XXXXXXX,        XXXXXXX,XXXXXXX
  ),

  [_SYMBOLS] = LAYOUT_4x7_right( // Hold T
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,KC_TILD,KC_PIPE, KC_EQL,KC_PLUS,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,KC_LPRN,KC_RPRN,KC_LBRC,KC_RBRC,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,KC_LABK,KC_RABK,KC_LCBR,KC_RCBR,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                                                XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
                                            KC_LSFT,KC_LCTL,                                XXXXXXX,
                                                    XXXXXXX,XXXXXXX,                XXXXXXX,
                                                    XXXXXXX,XXXXXXX,        XXXXXXX,XXXXXXX
  ),

  [_PUNCT] = LAYOUT_4x7_right( // Hold R
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,LGUI(KC_LEFT),LGUI(KC_RGHT),XXXXXXX,                    XXXXXXX,KC_EXLM, KC_AT ,KC_HASH, KC_DLR,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,KC_PERC,KC_CIRC,KC_AMPR,KC_ASTR,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                                                XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
                                            KC_LSFT,KC_LCTL,                                XXXXXXX,
                                                    XXXXXXX,XXXXXXX,                XXXXXXX,
                                                    XXXXXXX,XXXXXXX,        XXXXXXX,XXXXXXX
  ),

  [_CLICK] = LAYOUT_4x7_right( // Hold D
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,MS_BTN1,MS_BTN2,MS_BTN3,XXXXXXX,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                XXXXXXX,MS_BTN4,MS_BTN5,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                                                                XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
                                            KC_LSFT,KC_LCTL,                                XXXXXXX,
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

// RGB Matrix LED mapping
// TODO: Fill in LED indices based on your wiring order (serpentine)
// Left half: LEDs 0-31, Right half: LEDs 32-61
// Use NO_LED for matrix positions with no physical key/LED
#ifdef RGB_MATRIX_ENABLE
led_config_t g_led_config = { {
    // matrix_co: [row][col] → LED index
    // Left half (rows 0-4)
    { 0,      1,      2,      3,      4,      5,      6      },  // row 0 (L→R)
    { 13,     12,     11,     10,     9,      8,      7      },  // row 1 (R→L serpentine)
    { 14,     15,     16,     17,     18,     19,     20     },  // row 2 (L→R)
    { 25,     24,     23,     22,     21,     NO_LED, NO_LED },  // row 3 (R→L)
    { 26,     27,     28,     29,     30,     31,     NO_LED },  // row 4 thumb (L→R)
    // Right half (rows 5-9)
    { 32,     33,     34,     35,     36,     37,     38     },  // row 5 (L→R)
    { 45,     44,     43,     42,     41,     40,     39     },  // row 6 (R→L serpentine)
    { 46,     47,     48,     49,     50,     51,     52     },  // row 7 (L→R)
    { 57,     56,     55,     54,     53,     NO_LED, NO_LED },  // row 8 (R→L)
    { 58,     59,     60,     61,     NO_LED, NO_LED, NO_LED },  // row 9 thumb (L→R)
}, {
    // point: physical {x, y} position of each LED (0-224 x, 0-64 y)
    // Left half (LEDs 0-31) — TODO: adjust positions
    {0,0},  {16,0},  {32,0},  {48,0},  {64,0},  {80,0},  {96,0},   // row 0
    {96,16}, {80,16}, {64,16}, {48,16}, {32,16}, {16,16}, {0,16},   // row 1
    {0,32},  {16,32}, {32,32}, {48,32}, {64,32}, {80,32}, {96,32},  // row 2
    {80,48}, {64,48}, {48,48}, {32,48}, {16,48},                    // row 3
    {16,64}, {32,64}, {48,64}, {64,64}, {80,64}, {96,64},           // row 4 thumb
    // Right half (LEDs 32-61) — TODO: adjust positions
    {128,0}, {144,0}, {160,0}, {176,0}, {192,0}, {208,0}, {224,0},  // row 5
    {224,16},{208,16},{192,16},{176,16},{160,16},{144,16},{128,16},   // row 6
    {128,32},{144,32},{160,32},{176,32},{192,32},{208,32},{224,32},   // row 7
    {208,48},{192,48},{176,48},{160,48},{144,48},                    // row 8
    {144,64},{160,64},{176,64},{192,64},                             // row 9 thumb
}, {
    // flags: 4 = LED_FLAG_KEYLIGHT for all per-key LEDs
    4,4,4,4,4,4,4, 4,4,4,4,4,4,4, 4,4,4,4,4,4,4, 4,4,4,4,4, 4,4,4,4,4,4,  // left 0-31
    4,4,4,4,4,4,4, 4,4,4,4,4,4,4, 4,4,4,4,4,4,4, 4,4,4,4,4, 4,4,4,4,       // right 32-61
} };
#endif

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
