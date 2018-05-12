#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include <webots/robot.h>

#include "../util/motors.h"
#include "../util/ground_sensors.h"
#include "../util/com.h"
#include "../util/consts.h"

#include "../common/barcode.h"

#define MOTOR_SPEED     2.0
#define DATA_MAX_SIZE   100
#define SYNC_STOP_VALUE 0b111
#define BARS_PER_VALUE  6

int main()
{
    wb_robot_init();
    motors_init();
    ground_init();
    com_init();
    
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
            
            size_t index = step / BARS_PER_VALUE;
            data[index] |= input << (step % BARS_PER_VALUE);
            
            if(data[index] == SYNC_STOP_VALUE)
                break;
            
            ++step;
        }
    }
    
    motors_stop();
    
    const size_t data_size = step / BARS_PER_VALUE;
    
    printf("Done! (data: ");
    for(size_t i = 0; i < data_size; ++i)
        printf("%d%s", data[i], i == data_size - 1 ? ")\n" : ", ");
    
    printf("Sending packet...\n");
    packet_t packet = {data, data_size};
    if(com_send(packet))
        printf("Done!\n");
    else
        printf("Failed!\n");
    
    wb_robot_cleanup();
    
    return EXIT_SUCCESS;
}
