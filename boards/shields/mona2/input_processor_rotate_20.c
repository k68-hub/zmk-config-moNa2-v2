#include <kernel.h>
#include <zmk/input/input.h>
#include <zmk/input/input_processor.h>
#include <math.h>

#define DT_DRV_COMPAT zmk_input_processor_rotate_20

// 20度回転の三角関数値（倍率1000）
#define COS_20_DEG 940   // cos(20°) * 1000
#define SIN_20_DEG 342   // sin(20°) * 1000

struct rotate_20_config {
    int dummy;
};

struct rotate_20_data {
    int last_x;
    int last_y;
    bool x_updated;
};

static int rotate_20_process(const struct device *dev, struct input_event *evt) {
    struct rotate_20_data *data = dev->data;
    
    if (evt->code == INPUT_REL_X) {
        data->last_x = evt->value;
        data->x_updated = true;
        return 0;
    }
    
    if (evt->code == INPUT_REL_Y && data->x_updated) {
        data->last_y = evt->value;
        
        // 回転行列を適用
        int rotated_x = (data->last_x * COS_20_DEG - data->last_y * SIN_20_DEG) / 1000;
        int rotated_y = (data->last_x * SIN_20_DEG + data->last_y * COS_20_DEG) / 1000;
        
        evt->value = rotated_y;
        data->x_updated = false;
        
        // X値も更新する必要がある場合の処理
        // （ここでは Y のみ処理）
    }
    
    return 0;
}

static int rotate_20_init(const struct device *dev) {
    return 0;
}

static const struct input_processor_driver_api api = {
    .process = rotate_20_process,
};

static struct rotate_20_data rotate_20_data;
static const struct rotate_20_config rotate_20_config;

DEVICE_DT_INST_DEFINE(0, rotate_20_init, NULL,
                      &rotate_20_data, &rotate_20_config,
                      POST_KERNEL, CONFIG_INPUT_PROCESSOR_INIT_PRIORITY,
                      &api);
