/*
Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert


This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.


This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once


// wiring of each half
#define MATRIX_COL_PINS { A6, A5, A1, B13, B14, B15, B1 }
#define MATRIX_ROW_PINS { B8, A3, B10, A8, B0 }
#define DIODE_DIRECTION ROW2COL

#define DEBUG_MATRIX_SCAN_RATE
#define DEBUG_ENABLE
#define DEBUG_MATRIX


// WS2812 RGB LED strip input and number of LEDs
#define WS2812_DI_PIN A10
#define RGBLED_NUM 62
#define RGBLIGHT_SPLIT
#define RGBLED_SPLIT \
    { 32, 30 }
#define RGBLIGHT_LIMIT_VAL 80


//define AUDIO_PIN       C6


/* USART Full-Duplex configuration for split keyboard */
#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_TX_PIN A15
#define SERIAL_USART_RX_PIN B7
#define SERIAL_USART_TX_PAL_MODE 7
#define SERIAL_USART_RX_PAL_MODE 7

#define MASTER_LEFT
// #define EE_HANDS


/* OLED Configuration - DISABLED
 * To re-enable OLED, physically rewire to B6 (SCL) and B9 (SDA)
 * Then uncomment these lines and set OLED_ENABLE = yes in rules.mk:
 * 
 * #define I2C_DRIVER I2CD1
 * #define I2C1_SCL_PIN B6
 * #define I2C1_SDA_PIN B9
 * 
 * Note: Cannot use A15/B7 as they are used for USART split communication
 * Pin selection based on Bonsai C-4 pinout:
 * https://docs.google.com/spreadsheets/d/1FY-Vt8GbN7uX89lh9176jPXvGJELREzGpuhMts9ds38/edit
 */


/* SPI configuration for PMW3389 */
#define SPI_DRIVER SPID1
#define SPI_SCK_PIN B3
#define SPI_SCK_PAL_MODE 5
#define SPI_MOSI_PIN B5
#define SPI_MOSI_PAL_MODE 5
#define SPI_MISO_PIN B4
#define SPI_MISO_PAL_MODE 5


/* PMW33XX Settings */
#define PMW33XX_CS_PIN A7
#define PMW33XX_LIFTOFF_DISTANCE 0b0111



#define OLED_DISPLAY_128X32
#define OLED_BRIGHTNESS 200


#define CHARYBDIS_MINIMUM_DEFAULT_DPI 1200
#define CHARYBDIS_DEFAULT_DPI_CONFIG_STEP 200
#define CHARYBDIS_MINIMUM_SNIPING_DPI 400
#define CHARYBDIS_SNIPING_DPI_CONFIG_STEP 200

#define POINTING_DEVICE_RIGHT


// #define BOOTMAGIC_EEPROM_ROW          1
// #define BOOTMAGIC_EEPROM_COLUMN       0
// #define BOOTMAGIC_EEPROM_ROW_RIGHT    7
// #define BOOTMAGIC_EEPROM_COLUMN_RIGHT 5




// #define DEBOUNCE 15


// #define LAYER_MAP_REMAPPING
// #define LAYER_MAP_COLS 17
// #define LAYER_MAP_ROWS 8