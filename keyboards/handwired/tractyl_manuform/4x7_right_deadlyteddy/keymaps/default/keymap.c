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
#include <math.h>

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
#define HOME_SLSH LT(_MEDIA, KC_SLSH)
#define HOME_N LCTL_T(KC_N)

const uint16_t PROGMEM combo_f5[] = {HOME_A, HOME_R, HOME_S, HOME_T, COMBO_END};
const uint16_t PROGMEM combo_gui_ctl_0[] = {KC_LSFT, HOME_S, HOME_T, COMBO_END};
const uint16_t PROGMEM combo_boot[] = {KC_W, KC_F, KC_P, KC_B, COMBO_END};
const uint16_t PROGMEM combo_esc[] = {HOME_R, HOME_S, COMBO_END};
const uint16_t PROGMEM combo_adhd[] = {KC_H, KC_COMM, KC_DOT, HOME_SLSH, COMBO_END};
const uint16_t PROGMEM combo_f23[] = {HOME_N, KC_E, KC_I, COMBO_END};
const uint16_t PROGMEM combo_luy[] = {KC_L, KC_U, KC_Y, COMBO_END};

static uint16_t luy_timer = 0;

enum combo_names {
    COMBO_F5,
    COMBO_GUI_CTL_0,
    COMBO_BOOT,
    COMBO_ESC,
    COMBO_ADHD,
    COMBO_F23,
    COMBO_LUY,
    COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;

combo_t key_combos[] = {
    [COMBO_F5]        = COMBO(combo_f5, KC_F5),
    [COMBO_GUI_CTL_0] = COMBO(combo_gui_ctl_0, LGUI(LCTL(KC_0))),
    [COMBO_BOOT]      = COMBO(combo_boot, QK_BOOT),
    [COMBO_ESC]       = COMBO(combo_esc, KC_ESC),
    [COMBO_ADHD]      = COMBO(combo_adhd, LCTL(KC_F17)),
    [COMBO_F23]       = COMBO(combo_f23, KC_F23),
    [COMBO_LUY]       = COMBO_ACTION(combo_luy),
};

bool get_combo_must_hold(uint16_t index, combo_t *combo) {
    return false;
}

void process_combo_event(uint16_t combo_index, bool pressed) {
    if (combo_index == COMBO_LUY && pressed) {
        tap_code16(LCTL(LSFT(KC_0)));
        luy_timer = timer_read();
    }
}

// Short aliases

// Custom tap/hold keycodes — tap outputs immediately on release, hold after TAPPING_TERM
enum custom_keycodes {
    TH_MINS_ENT = SAFE_RANGE,
    TH_LCLK_RCLK,
    TH_LOCK_CAD,
    Z_ARROWS_SHIFTCTRL,
    OLED_L,
    OLED_R,
};

enum oled_mode {
    OLED_MODE_OFF,
    OLED_MODE_STATUS,
    OLED_MODE_STATS,
    OLED_MODE_BONGO,
    OLED_MODE_LUNA,
    OLED_MODE_RGB,
    OLED_MODE_COUNT,
};

static uint8_t left_oled_mode = OLED_MODE_STATUS;
static uint8_t right_oled_mode = OLED_MODE_STATS;

// Store RGB state before switching to QWERTY
static uint8_t saved_rgb_mode = 0;
static uint8_t saved_rgb_hue = 0;
static uint8_t saved_rgb_sat = 0;
static uint8_t saved_rgb_val = 0;

typedef struct {
    uint16_t tap_kc;
    uint16_t hold_kc;
    uint16_t timer;
    bool held;
} tap_hold_t;

static tap_hold_t tap_holds[] = {
    [TH_MINS_ENT - SAFE_RANGE]   = {KC_MINS, KC_ENT,  0, false},
    [TH_LCLK_RCLK - SAFE_RANGE]  = {MS_BTN1, MS_BTN2, 0, false},
    [TH_LOCK_CAD - SAFE_RANGE]   = {LWIN(KC_L), LCTL(LALT(KC_DEL)), 0, false},
};

static struct {
    uint16_t timer;
    bool held;
} z_state = {0, false};

static uint32_t ee_clear_timer = 0;
static uint32_t keypress_count = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        keypress_count++;
        if (keypress_count % 1000 == 0) eeconfig_update_user(keypress_count);
    }

    // EEPROM clear: hold bottom corner key for 5 seconds
    if (keycode == KC_F20 || keycode == KC_F24) {
        ee_clear_timer = record->event.pressed ? timer_read32() : 0;
    }

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
        if (!record->event.pressed && z_state.held) {
            layer_off(_ARROWS);
            unregister_code(KC_LSFT);
            unregister_code(KC_LCTL);
            z_state.held = false;
            z_state.timer = 0;
            return false;
        }

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
            } else {
                layer_off(_ARROWS);
                unregister_code(KC_LSFT);
                unregister_code(KC_LCTL);
            }
            z_state.held = false;
            z_state.timer = 0;
        }
        return false;
    }

    if (record->event.pressed) {
        if (keycode == OLED_L) {
            left_oled_mode = (left_oled_mode + 1) % OLED_MODE_COUNT;
            return false;
        }
        if (keycode == OLED_R) {
            right_oled_mode = (right_oled_mode + 1) % OLED_MODE_COUNT;
            return false;
        }
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

    if (luy_timer && timer_elapsed(luy_timer) > 200) {
        tap_code16(LCTL(LSFT(KC_O)));
        luy_timer = 0;
    }

    if (ee_clear_timer && timer_elapsed32(ee_clear_timer) > 5000) {
        ee_clear_timer = 0;
        eeconfig_init();
        soft_reset_keyboard();
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    static bool was_on_qwerty = false;
    bool is_on_qwerty = IS_LAYER_ON_STATE(state, _QWERTY);

    if (is_on_qwerty && !was_on_qwerty) {
        // Entering QWERTY - save current RGB state
        saved_rgb_mode = rgb_matrix_get_mode();
        saved_rgb_hue = rgb_matrix_get_hue();
        saved_rgb_sat = rgb_matrix_get_sat();
        saved_rgb_val = rgb_matrix_get_val();

        // Set QWERTY RGB
        rgb_matrix_mode_noeeprom(3);
        rgb_matrix_sethsv_noeeprom(111, 255, 120);
    } else if (!is_on_qwerty && was_on_qwerty) {
        // Leaving QWERTY - restore previous RGB state
        rgb_matrix_mode_noeeprom(saved_rgb_mode);
        rgb_matrix_sethsv_noeeprom(saved_rgb_hue, saved_rgb_sat, saved_rgb_val);
    }

    was_on_qwerty = is_on_qwerty;
    return state;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_COLEMAK_DH] = LAYOUT_4x7_right(
    KC_ESC ,KC_DEL , KC_Q  , KC_W  , KC_F  , KC_P  , KC_B  ,                                 KC_J  , KC_L  , KC_U  , KC_Y  ,KC_SCLN,KC_MINS,TH_LOCK_CAD,
    KC_F18 ,KC_BSPC, HOME_A, HOME_R, HOME_S, HOME_T, KC_G  ,                                 KC_M  , HOME_N, KC_E  , KC_I  , KC_O  ,KC_QUOT,KC_F22 ,
    KC_F19 ,KC_LCTL, Z_ARROWS_SHIFTCTRL, KC_X  , KC_C  , HOME_D, KC_V  ,                                 KC_K  , KC_H  ,KC_COMM, KC_DOT,HOME_SLSH,KC_GRV,KC_F23 ,
    KC_F20 ,KC_F21 ,KC_LWIN,KC_LALT,LCTL(KC_BSPC),                                                                KC_MINS, KC_EQL ,KC_BSLS,TG(_QWERTY),KC_F24 ,
                                            KC_LSFT,KC_LCTL,                                 KC_SPC,
                                                    KC_LALT, KC_TAB,                 KC_MINS,
                                                    SNIPING,KC_ESC ,          KC_F17, KC_ENT
  ),

  [_NUMS] = LAYOUT_4x7_right( // Hold S
    _______,_______,_______,_______,_______,_______,_______,                                _______, KC_7  , KC_8  , KC_9  ,KC_EQL ,_______,_______,
    _______,_______,KC_BSPC,_______,_______,KC_LSFT,_______,                                _______, KC_4  , KC_5  , KC_6  ,KC_ASTR,_______,_______,
    _______,_______,_______,_______,_______,_______,_______,                                _______, KC_1  , KC_2  , KC_3  ,KC_SLSH,_______,_______,
    _______,_______,_______,_______,_______,                                                                KC_MINS,KC_PLUS,_______,_______,_______,
                                            LSFT_T(KC_SPC),KC_LCTL,                           KC_0 ,
                                                    KC_LALT,_______,                _______,
                                                    _______,_______,        _______,_______
  ),

  [_ARROWS] = LAYOUT_4x7_right( // Hold A
    _______,_______,_______,_______,_______,_______,_______,                                _______,_______,KC_PGDN,KC_PGUP,_______,_______,_______,
    _______,_______,_______,_______,_______,_______,_______,                                KC_HOME,KC_LEFT,KC_DOWN, KC_UP ,KC_RGHT,KC_END ,_______,
    _______,_______,_______,_______,_______,_______,_______,                                _______,_______,_______,_______,_______,_______,_______,
    _______,_______,_______,_______,_______,                                                                _______,_______,_______,_______,_______,
                                            KC_LSFT,KC_LCTL,                                KC_LSFT,
                                                    _______,_______,                KC_LCTL,
                                                    _______,_______,        _______,_______
  ),

  [_SYMBOLS] = LAYOUT_4x7_right( // Hold T
    _______,_______,_______,_______,_______,_______,_______,                                _______,KC_TILD,KC_PIPE, KC_EQL,KC_PLUS,_______,_______,
    _______,_______,_______,_______,_______,_______,_______,                                _______,KC_LPRN,KC_RPRN,KC_LBRC,KC_RBRC,_______,_______,
    _______,_______,_______,_______,_______,_______,_______,                                _______,KC_LABK,KC_RABK,KC_LCBR,KC_RCBR,_______,_______,
    _______,_______,_______,_______,_______,                                                                _______,_______,_______,_______,_______,
                                            KC_LSFT,KC_LCTL,                                _______,
                                                    _______,_______,                _______,
                                                    _______,_______,        _______,_______
  ),

  [_PUNCT] = LAYOUT_4x7_right( // Hold R
    _______,_______,_______,_______,_______,_______,_______,                                _______,_______,_______,_______,_______,_______,_______,
    _______,_______,_______,_______,LGUI(KC_LEFT),LGUI(KC_RGHT),_______,                    _______,KC_EXLM, KC_AT ,KC_HASH, KC_DLR,_______,_______,
    _______,_______,_______,_______,_______,_______,_______,                                _______,KC_PERC,KC_CIRC,KC_AMPR,KC_ASTR,_______,_______,
    _______,_______,_______,_______,_______,                                                                _______,_______,_______,_______,_______,
                                            KC_LSFT,KC_LCTL,                                _______,
                                                    _______,_______,                _______,
                                                    _______,_______,        _______,_______
  ),

  [_CLICK] = LAYOUT_4x7_right( // Hold D
    _______,_______,_______,_______,_______,_______,_______,                                _______,_______,_______,_______,_______,_______,_______,
    _______,_______,_______,_______,_______,_______,_______,                                _______,MS_BTN1,MS_BTN2,MS_BTN3,_______,_______,_______,
    _______,_______,_______,_______,_______,_______,_______,                                _______,MS_BTN4,MS_BTN5,_______,_______,_______,_______,
    _______,_______,_______,_______,_______,                                                                _______,_______,_______,_______,_______,
                                            KC_LSFT,KC_LCTL,                                _______,
                                                    _______,_______,                _______,
                                                    _______,_______,        _______,_______
  ),

  [_MEDIA] = LAYOUT_4x7_right( // Hold '/'
    _______,_______,_______,KC_VOLD,KC_VOLU,KC_MUTE,_______,                                _______,RM_HUEU,RM_VALU,RM_SATU,_______,_______,_______,
    _______,_______,_______,KC_MNXT,KC_MPRV,KC_MPLY,_______,                                _______,RM_HUED,RM_VALD,RM_SATD,_______,_______,_______,
    _______,_______,_______,_______,OLED_L ,OLED_R ,_______,                                _______,RM_TOGG,RM_PREV,RM_NEXT,_______,_______,_______,
    _______,_______,_______,_______,_______,                                                                _______,_______,_______,_______,_______,
                                            _______,_______,                                _______,
                                                    _______,_______,                _______,
                                                    _______,_______,        _______,_______
  ),

  [_QWERTY] = LAYOUT_4x7_right(
    KC_ESC ,KC_TAB , KC_Q  , KC_W  , KC_E  , KC_R  , KC_T  ,                                 KC_Y  , KC_U  , KC_I  , KC_O  , KC_P  ,KC_MINS,TH_LOCK_CAD,
    KC_F18 ,KC_BSPC, KC_A  , KC_S  , KC_D  , KC_F  , KC_G  ,                                 KC_H  , KC_J  , KC_K  , KC_L  ,KC_SCLN,KC_QUOT,KC_F22 ,
    KC_F19 ,KC_LSFT, KC_Z  , KC_X  , KC_C  , KC_V  , KC_B  ,                                 KC_N  , KC_M  ,KC_COMM, KC_DOT,KC_SLSH, KC_GRV,KC_F23 ,
    KC_F20 ,KC_F21 ,KC_LWIN,KC_LALT, KC_M  ,                                                                KC_MINS, KC_EQL ,KC_BSLS,TG(_QWERTY),KC_F24 ,
                                            KC_SPC ,KC_LCTL,                                 KC_SPC,
                                                    KC_LALT, KC_TAB,                   TH_MINS_ENT,
                                                    KC_BSPC,KC_LWIN,          TH_LCLK_RCLK, KC_ENT
  ),

};

// OLED displays
#ifdef OLED_ENABLE
#include "bongocat.h"
#include "luna.h"
#include "transactions.h"

static void oled_mode_sync_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    const uint8_t *d = (const uint8_t*)in_data;
    right_oled_mode = d[0];
    memcpy(&keypress_count, &d[1], sizeof(keypress_count));
}

void eeconfig_init_user(void) {
    eeconfig_update_user(0);
}

void keyboard_post_init_user(void) {
    keypress_count = eeconfig_read_user();
    transaction_register_rpc(OLED_SYNC_ID, oled_mode_sync_handler);
}


void suspend_power_down_user(void) {
    oled_off();
    eeconfig_update_user(keypress_count);
}
void housekeeping_task_user(void) {
    if (is_keyboard_master()) {
        static uint8_t last_mode = 0xFF;
        static uint32_t last_keys = 0xFFFFFFFF;
        if (right_oled_mode != last_mode || keypress_count != last_keys) {
            uint8_t buf[5];
            buf[0] = right_oled_mode;
            memcpy(&buf[1], &keypress_count, sizeof(keypress_count));
            if (transaction_rpc_send(OLED_SYNC_ID, sizeof(buf), buf)) {
                last_mode = right_oled_mode;
                last_keys = keypress_count;
            }
        }
    }
}

static oled_rotation_t current_rotation = OLED_ROTATION_90;

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return current_rotation;
}

static void set_rotation(oled_rotation_t rot) {
    if (rot != current_rotation) {
        oled_clear();
        oled_render_dirty(true);
        current_rotation = rot;
        oled_init(rot);
        oled_clear();
        oled_render_dirty(true);
    }
}

static void render_status(void) {
    char buf[12];
    oled_set_cursor(0, 1);

    // Layer
    oled_write("Layer", false);
    switch (get_highest_layer(layer_state)) {
        case _COLEMAK_DH: oled_write("COLMK", false); break;
        case _NUMS:       oled_write(" NUMS", false); break;
        case _ARROWS:     oled_write("ARROW", false); break;
        case _SYMBOLS:    oled_write("SYMBL", false); break;
        case _PUNCT:      oled_write("PUNCT", false); break;
        case _CLICK:      oled_write("CLICK", false); break;
        case _MEDIA:      oled_write("MEDIA", false); break;
        case _QWERTY:     oled_write("QWRTY", false); break;
        default:          oled_write("?????", false); break;
    }
    oled_write("-----", false);
    oled_write("     ", false);

    // WPM
    oled_write_ln("WPM", false);
    snprintf(buf, sizeof(buf), "  %3d", get_current_wpm());
    oled_write(buf, false);
    oled_write("-----", false);
    oled_write("     ", false);

    // Keys
     oled_write_ln("Keys", false);
    if (keypress_count > 99999) {
        snprintf(buf, sizeof(buf), "%4luk", (unsigned long)(keypress_count / 1000));
    } else {
        snprintf(buf, sizeof(buf), "%5lu", (unsigned long)keypress_count);
    }
    oled_write_ln(buf, false);
    oled_write_ln("-----", false);

    // Mods
    oled_write_ln("Mods", false);
    uint8_t mods = get_mods() | get_oneshot_mods();
    snprintf(buf, sizeof(buf), " %c%c%c%c",
        (mods & MOD_MASK_SHIFT) ? 'S' : ' ',
        (mods & MOD_MASK_CTRL)  ? 'C' : ' ',
        (mods & MOD_MASK_ALT)   ? 'A' : ' ',
        (mods & MOD_MASK_GUI)   ? 'G' : ' ');
    oled_write(buf, false);
}

static void render_stats(void) {
    char buf[12];
    oled_set_cursor(0, 1);
    oled_write_ln("Up", false);
    uint32_t secs = timer_read32() / 1000;
    uint16_t hrs = secs / 3600;
    uint8_t mins = (secs / 60) % 60;
    snprintf(buf, sizeof(buf), "%02d:%02d", hrs > 99 ? 99 : (int)hrs, mins);
    oled_write_ln(buf, false);
    oled_write_ln("-----", false);
    oled_write_ln("Keys", false);
    if (keypress_count > 99999) {
        snprintf(buf, sizeof(buf), "%4luk", (unsigned long)(keypress_count / 1000));
    } else {
        snprintf(buf, sizeof(buf), "%5lu", (unsigned long)keypress_count);
    }
    oled_write_ln(buf, false);
    oled_write_ln("-----", false);
    oled_write_ln("RGB", false);
    if (rgb_matrix_is_enabled()) {
        snprintf(buf, sizeof(buf), "  %2d", rgb_matrix_get_mode());
        oled_write_ln(buf, false);
    } else {
        oled_write_ln(" OFF", false);
    }
}

static void render_rgb(void) {
    char buf[12];
    static bool prev_rgb_on = true;
    bool rgb_on = rgb_matrix_is_enabled();
    if (rgb_on != prev_rgb_on) {
        prev_rgb_on = rgb_on;
        oled_clear();
        oled_render_dirty(true);
    }
    oled_set_cursor(0, 1);
    oled_write_ln("RGB", false);
    if (!rgb_on) {
        oled_write_ln(" OFF", false);
        return;
    }
    snprintf(buf, sizeof(buf), " %3d", rgb_matrix_get_mode());
    oled_write_ln(buf, false);
    oled_write_ln("-----", false);
    oled_write_ln("Hue", false);
    snprintf(buf, sizeof(buf), " %3d", rgb_matrix_get_hue());
    oled_write_ln(buf, false);
    oled_write_ln("-----", false);
    oled_write_ln("Sat", false);
    snprintf(buf, sizeof(buf), " %3d", rgb_matrix_get_sat());
    oled_write_ln(buf, false);
    oled_write_ln("-----", false);
    oled_write_ln("Brt", false);
    snprintf(buf, sizeof(buf), " %3d", rgb_matrix_get_val());
    oled_write_ln(buf, false);
}

bool oled_task_user(void) {
    uint8_t mode = is_keyboard_master() ? left_oled_mode : right_oled_mode;
    static uint8_t prev_mode = 0xFF;

    // Full clear + flush on mode change
    if (mode != prev_mode) {
        prev_mode = mode;
        if (mode == OLED_MODE_BONGO) {
            set_rotation(is_keyboard_master() ? OLED_ROTATION_180 : OLED_ROTATION_0);
        } else {
            set_rotation(OLED_ROTATION_90);
        }
        oled_clear();
        oled_render_dirty(true);
    }

    if (mode == OLED_MODE_OFF) return false;

    switch (mode) {
        case OLED_MODE_STATUS:
            render_status();
            break;
        case OLED_MODE_STATS:
            render_stats();
            break;
        case OLED_MODE_BONGO:
            render_bongocat();
            break;
        case OLED_MODE_LUNA:
            render_luna();
            break;
        case OLED_MODE_RGB:
            render_rgb();
            break;
    }
    return false;
}
#endif

// RGB Matrix LED mapping
// TODO: Fill in LED indices based on your wiring order (serpentine)
// Left half: LEDs 0-31, Right half: LEDs 32-61
// Use NO_LED for matrix positions with no physical key/LED
#ifdef RGB_MATRIX_ENABLE
led_config_t g_led_config = { {
    // matrix_co: [row][col] → LED index
    // Left half (rows 0-4)
    { 6,      5,      4,      3,      2,      1,      0      },  // row 0 (L→R)
    { 7,      8,      9,      10,     11,     12,     13     },  // row 1 (R→L serpentine)
    { 20,     19,     18,     17,     16,     15,     14     },  // row 2 (L→R)
    { 21,     22,     23,     24,     25,     26,     27     },  // row 3 (R→L)
    { NO_LED, NO_LED, 28,     29,     31,     30,     NO_LED },  // row 4 thumb (L→R)
    // Right half (rows 5-9)
    { 32,     33,     34,     35,     36,     37,     38     },  // row 5 (L→R)
    { 45,     44,     43,     42,     41,     40,     39     },  // row 6 (R→L serpentine)
    { 46,     47,     48,     49,     50,     51,     52     },  // row 7 (L→R)
    { NO_LED, 58,     57,     56,     55,     54,     53     },  // row 8 (R→L)
    { NO_LED, 59,     61,     60,     NO_LED, NO_LED, NO_LED },  // row 9 thumb (L→R)
}, {
    // point: physical {x, y} position of each LED in wiring order
    // Left half (LEDs 0-31)
    {96,0},  {80,0},  {64,0},  {48,0},  {32,0},  {16,0},  {0,0},    // row 0: L06→L00 (R→L)
    {0,16},  {16,16}, {32,16}, {48,16}, {64,16}, {80,16}, {96,16},   // row 1: L10→L16 (L→R)
    {96,32}, {80,32}, {64,32}, {48,32}, {32,32}, {16,32}, {0,32},    // row 2: L26→L20 (R→L)
    {0,48},  {16,48}, {32,48}, {48,48}, {64,48}, {80,48}, {96,48},   // row 3: L30→L36 (L→R)
                      {48,64}, {64,64}, {96,64}, {80,64},                               // row 4: L42,L43,L45,L44
    // Right half (LEDs 32-61)
    {128,0}, {144,0}, {160,0}, {176,0}, {192,0}, {208,0}, {224,0},   // row 5: R00→R06 (L→R)
    {224,16},{208,16},{192,16},{176,16},{160,16},{144,16},{128,16},    // row 6: R16→R10 (R→L)
    {128,32},{144,32},{160,32},{176,32},{192,32},{208,32},{224,32},    // row 7: R20→R26 (L→R)
             {224,48},{208,48},{192,48},{176,48},{160,48},{144,48},             // row 8: R36→R31 (R→L)
             {144,64},{176,64},{160,64},                                        // row 9: R41,R43,R42
}, {
    4,4,4,4,4,1,1,  // left row 0 (7)
    1,1,4,4,4,4,4,  // left row 1 (7)
    4,4,4,4,4,1,1,  // left row 2 (7)
    1,1,1,1,1,1,1,  // left row 3 (7)
    1,1,1,1,        // left row 4 thumb (4)
    4,4,4,4,4,1,1,  // right row 5 (7)
    1,1,4,4,4,4,4,  // right row 6 (7)
    4,4,4,4,4,1,1,  // right row 7 (7)
    1,1,1,1,1,1,    // right row 8 (6)
    1,1,1,          // right row 9 thumb (3)
} };
#endif

// Trackball mods
#define TRACKBALL_ANGLE 17 // degrees, positive=CCW, negative=CW

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    float rad = TRACKBALL_ANGLE * M_PI / 180.0f;
    int16_t x = mouse_report.x;
    int16_t y = mouse_report.y;
    mouse_report.x = (x * cosf(rad) - y * sinf(rad)) * 2.5;
    mouse_report.y = -(x * sinf(rad) + y * cosf(rad));
    return mouse_report;
}

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
