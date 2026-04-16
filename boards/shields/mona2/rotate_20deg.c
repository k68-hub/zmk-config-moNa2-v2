#include <kernel.h>
#include <zmk/input/input.h>

#define DT_DRV_COMPAT zmk_input_processor_rotate_20deg

// 20度回転の三角関数値（倍率1000）
#define COS_20_DEG 940   // cos(20°) * 1000
#define SIN_20_DEG 342   // sin(20°) * 1000

static int last_x = 0;
static int last_y = 0;
static bool has_x = false;

static int rotate_20deg_process(const struct device *dev, struct input_event evt) {
    if (evt.code == INPUT_REL_X) {
        last_x = evt.value;
        has_x = true;
        return 0;
    }
    
    if (evt.code == INPUT_REL_Y && has_x) {
        last_y = evt.value;
        
        // 回転行列を適用: [x'] = [cos(θ) -sin(θ)] [x]
        //                [y']   [sin(θ)  cos(θ)] [y]
        int rotated_x = (last_x * COS_20_DEG - last_y * SIN_20_DEG) / 1000;
        int rotated_y = (last_x * SIN_20_DEG + last_y * COS_20_DEG) / 1000;
        
        last_x = rotated_x;
        last_y = rotated_y;
        
        has_x = false;
    }
    
    return 0;
}

static const struct input_processor_driver_api api = {
    .process = rotate_20deg_process,
};

#define ROTATE_20DEG_INST(n) \
    DEVICE_DT_INST_DEFINE(n, NULL, NULL, NULL, NULL, POST_KERNEL, \
                          CONFIG_INPUT_PROCESSOR_INIT_PRIORITY, &api);

DT_INST_FOREACH_STATUS_OKAY(ROTATE_20DEG_INST)
