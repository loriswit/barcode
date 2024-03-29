#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <webots/robot.h>

#include "../util/motors.h"
#include "../util/ground_sensors.h"
#include "../util/leds.h"
#include "../util/com.h"
#include "../util/consts.h"

#include "../common/barcode.h"
#include "../common/helpers.h"

#define MOTOR_SPEED     1.5
#define DATA_MAX_SIZE   100
#define SYNC_STOP_VALUE 0b111
#define BARS_PER_VALUE  6

int main()
{
    wb_robot_init();
    motors_init();
    ground_init();
    leds_init();
    com_init();
    
    motors_set_speed(MOTOR_SPEED, MOTOR_SPEED);
    
    double time_per_step = sync();
    double step_start = now() + time_per_step / 3;
    
    uint8_t data[DATA_MAX_SIZE] = {0};
    size_t step = 0;
    
    println("Reading barcode...");
    
    while(wb_robot_step(TIME_STEP) != -1)
    {
        if(now() - step_start >= time_per_step)
        {
            step_start = now();
            color_t input = get_color();
            
            size_t index = step / BARS_PER_VALUE;
            data[index] |= input << (step % BARS_PER_VALUE);
            
            if(data[index] == SYNC_STOP_VALUE)
                break;
            
            ++step;
            leds_set(true);
        }
        else
            leds_set(false);
    }
    
    motors_stop();
    
    const size_t data_size = step / BARS_PER_VALUE;
    
    print("Done! (data: ");
    for(size_t i = 0; i < data_size; ++i)
        printf("%d%s", data[i], i == data_size - 1 ? ")\n" : ", ");
    
    println("Sending packet...");
    packet_t packet = {data, data_size};
    if(com_send(packet))
        println("Done!");
    else
        println("Failed!");
    
    wb_robot_cleanup();
    
    return EXIT_SUCCESS;
}
