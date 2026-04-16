#include <zephyr/kernel.h>
#include <zephyr/input/input.h>

#define DT_DRV_COMPAT zmk_input_processor_rotate_20

// 20度回転の三角関数値（倍率1000）
#define COS_20_DEG 940   // cos(20°) * 1000
#define SIN_20_DEG 342   // sin(20°) * 1000

struct rotate_20_data {
    int last_x;
    int last_y;
    bool has_x;
    bool has_y;
};

static struct rotate_20_data processor_data = {
    .last_x = 0,
    .last_y = 0,
    .has_x = false,
    .has_y = false,
};

static int rotate_20_process(const struct device *dev, struct input_event *evt) {
    struct rotate_20_data *data = &processor_data;
    
    if (evt->code == INPUT_REL_X) {
        data->last_x = evt->value;
        data->has_x = true;
        return 0;
    }
    
    if (evt->code == INPUT_REL_Y && data->has_x) {
        data->last_y = evt->value;
        
        // 回転行列を適用
        int rotated_x = (data->last_x * COS_20_DEG - data->last_y * SIN_20_DEG) / 1000;
        int rotated_y = (data->last_x * SIN_20_DEG + data->last_y * COS_20_DEG) / 1000;
        
        evt->value = rotated_y;
        data->has_x = false;
    }
    
    return 0;
}

static int rotate_20_init(const struct device *dev) {
    return 0;
}

static const struct input_processor_driver_api api = {
    .process = rotate_20_process,
};

DEVICE_DT_INST_DEFINE(0, rotate_20_init, NULL,
                      &processor_data, NULL,
                      POST_KERNEL, 99,
                      &api);
