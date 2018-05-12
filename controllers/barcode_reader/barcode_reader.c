#include <stdio.h>
#include <time.h>
#include <stdint.h>

#include <webots/robot.h>

#include "../util/motors.h"
#include "../util/ground_sensors.h"
#include "../util/consts.h"

#include "../common/barcode.h"

#define MOTOR_SPEED     2.0
#define DATA_MAX_SIZE   100
#define SYNC_STOP_VALUE 0b111

int main()
{
    wb_robot_init();
    motors_init();
    ground_init();
    
    motors_set_speed(MOTOR_SPEED, MOTOR_SPEED);
    
    clock_t ticks_per_step = sync();
    clock_t step_start = clock() + ticks_per_step / 2;
    
    uint8_t data[DATA_MAX_SIZE] = {0};
    size_t step = 0;
    
    printf("Reading barcode...\n");
    
    while(wb_robot_step(TIME_STEP) != -1)
    {
        if(clock() - step_start >= ticks_per_step)
        {
            step_start = clock();
            color_t input = get_color();
            
            size_t index = step / 3;
            data[index] |= input << (step % 3);
            
            if(index % 2 == 0 && data[index] == SYNC_STOP_VALUE)
                break;
            
            ++step;
        }
    }
    
    const size_t data_size = step / 3;
    
    printf("Done! (");
    for(size_t i = 0; i < data_size; ++i)
        printf("%d%s", data[i], i == data_size - 1 ? ")\n" : "-");
    
    motors_stop();
    wb_robot_cleanup();
    
    return EXIT_SUCCESS;
}
