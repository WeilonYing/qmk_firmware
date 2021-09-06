/*
QMK Firmware Massdrop CTRL M-AS Keymap

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// !!! DO NOT ADD #pragma once !!! //

// Step 1.
// Declare custom effects using the RGB_MATRIX_EFFECT macro
// (note the lack of semicolon after the macro!)

#include "animation.h"
RGB_MATRIX_EFFECT(CUSTOM_ANIM)

// Step 2.
// Define effects inside the `RGB_MATRIX_CUSTOM_EFFECT_IMPLS` ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#define LED_KEYBOARD_WIDTH 224
#define LED_KEYBOARD_HEIGHT 64

typedef HSV (*i_f_16time)(HSV hsv, uint8_t i, uint16_t time);

// using 16-bit time counter
/*
bool effect_runner_i_16time(effect_params_t* params, i_f_16time effect_func) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint16_t time = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed / 4, 1));
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        RGB rgb = rgb_matrix_hsv_to_rgb(effect_func(rgb_matrix_config.hsv, i, time));
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}
*/
typedef struct _coordinates {
    uint8_t x;
    uint8_t y;
} COORDINATES;

static uint8_t width_divisor(void) {
    return LED_KEYBOARD_WIDTH / ANIM_FRAME_WIDTH;
}

static uint8_t height_divisor(void) {
    return LED_KEYBOARD_HEIGHT / ANIM_FRAME_HEIGHT;
}
static COORDINATES map_key(COORDINATES key) {
    uint8_t x_out = (key.x / width_divisor());
    uint8_t y_out = (key.y / height_divisor());
    if (x_out < 0) { x_out = 0; }
    if (y_out < 0) { y_out = 0; }
    if (x_out >= ANIM_FRAME_WIDTH)  { x_out = ANIM_FRAME_WIDTH - 1; }
    if (y_out >= ANIM_FRAME_HEIGHT) { y_out = ANIM_FRAME_HEIGHT - 1; }

    COORDINATES output = {x_out, y_out};
    return output;
}
/*
static uint8_t abs_diff(uint8_t a, uint8_t b) {
    if (a > b) {
        return a - b;
    }
    return b - a;
}
*/

uint8_t multiplier;
bool multiplier_incremented; // prevent multiple increments in one go

static void HSV_CUSTOM_ANIM_init(void) {
    multiplier = 0;
    multiplier_incremented = 0;
    rgblight_set_speed(7);
}

static HSV CUSTOM_ANIM_math(HSV hsv, uint8_t i, uint8_t time) {
    COORDINATES key_coord;
    key_coord.x = g_led_config.point[i].x;
    key_coord.y = g_led_config.point[i].y;

    COORDINATES mapped_coord = map_key(key_coord);

    if (i == 0 && time == 0 && !multiplier_incremented) {
        multiplier++;
        multiplier_incremented = 1;
    }

    if (time == 250) { // prepare for increment
        multiplier_incremented = 0;
    }

    uint16_t index = ((multiplier * 256) + time) % ANIM_FRAME_COUNT;
#ifdef CONSOLE_ENABLE
    if (i == 10) {
        uprintf("x = %u, y = %u, index = %u\n", mapped_coord.x, mapped_coord.y, index);
    }
#endif
    bool result = g_frames.frames[index][mapped_coord.y][mapped_coord.x];
    hsv.h = 0;
    hsv.s = 0;
    if (result) {
        hsv.v = 255;
    } else {
        hsv.v = 0;
    }
    return hsv;
}
static bool CUSTOM_ANIM(effect_params_t* params) {
    if (params->init) {
        HSV_CUSTOM_ANIM_init();
    }
    return effect_runner_i(params, &CUSTOM_ANIM_math);
}

#endif  // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
