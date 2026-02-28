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

#pragma once

#include "tractyl_manuform.h"

#if defined(KEYBOARD_handwired_tractyl_manuform_4x7_right_elite_c)
#    include "elite_c.h"
#endif

// #include "quantum.h"

#define ___ KC_NO

// clang-format off
  /*REMOVE     0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18 --AXIS NOTES FOR info.json */
#define LAYOUT_4x7_right(\
  /*REMOVE 0*/L00, L01, L02, L03, L04, L05, L06,                          R00, R01, R02, R03, R04, R05, R06\
  /*REMOVE 1*/L10, L11, L12, L13, L14, L15, L16,                          R10, R11, R12, R13, R14, R15, R16\
  /*REMOVE 2*/L20, L21, L22, L23, L24, L25, L26,                          R20, R21, R22, R23, R24, R25, R26\
  /*REMOVE 3*/L30, L31, L32, L33, L34,                                              R32, R33, R34, R35, R36\
  /*REMOVE 4*/                         L35, L36,                               R31,                        \
  /*REMOVE 5*/                                   L44, L45,           R41,                                  \
  /*REMOVE 6*/                                   L42, L43,      R42, R43                                   \
  ) \
  { \
    { L00, L01, L02, L03, L04, L05, L06 }, \
    { L10, L11, L12, L13, L14, L15, L16 }, \
    { L20, L21, L22, L23, L24, L25, L26 }, \
    { L30, L31, L32, L33, L34, L35, L36 }, \
    { ___, ___, L42, L43, L44, L45, ___ }, \
                                     \
    { R00, R01, R02, R03, R04, R05, R06 }, \
    { R10, R11, R12, R13, R14, R15, R16 }, \
    { R20, R21, R22, R23, R24, R25, R26 }, \
    { ___, R31, R32, R33, R34, R35, R36 }, \
    { ___, R41, R42, R43, ___, ___, ___ }, \
}
// clang-format on