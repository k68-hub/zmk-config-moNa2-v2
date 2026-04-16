#include <kernel.h>
#include <drivers/sensor.h>
#include <zmk/input/input.h>

// 20度回転の三角関数値（倍率1000）
#define COS_20_DEG 940   // cos(20°) * 1000
#define SIN_20_DEG 342   // sin(20°) * 1000

static int last_x = 0;
static int last_y = 0;

static void rotate_20deg_process(struct input_event *evt) {
    if (evt->code == INPUT_REL_X) {
        last_x = evt->value;
    } else if (evt->code == INPUT_REL_Y) {
        last_y = evt->value;
        
        // 回転行列を適用
        int rotated_x = (last_x * COS_20_DEG - last_y * SIN_20_DEG) / 1000;
        int rotated_y = (last_x * SIN_20_DEG + last_y * COS_20_DEG) / 1000;
        
        last_x = rotated_x;
        last_y = rotated_y;
        
        evt->value = rotated_y;
    }
}

ZMK_INPUT_PROCESSOR_HANDLER(rotate_20deg, rotate_20deg_process);
