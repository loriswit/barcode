#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <webots/robot.h>

#include "../util/motors.h"
#include "../util/leds.h"
#include "../util/com.h"
#include "../util/consts.h"

#include "../common/helpers.h"

#define MOTOR_SPEED     2.0
#define DATA_MAX_SIZE   100

#define MOVE_COOLDOWN   (int)(3 * CLOCKS_PER_SEC / MOTOR_SPEED)
#define ROTATE_COOLDOWN (int)(1.17 * CLOCKS_PER_SEC / MOTOR_SPEED)

int main()
{
    wb_robot_init();
    motors_init();
    leds_init();
    com_init();
    
    println("Waiting for packet...");
    packet_t packet = {NULL, 0};
    
    while(wb_robot_step(TIME_STEP) != -1 && packet.data == NULL)
        packet = com_receive();
    
    println("Done! (received: %zu bytes)", packet.size);
    
    uint8_t * data = packet.data;
    size_t index = 0;
    clock_t cooldown = clock();
    
    while(wb_robot_step(TIME_STEP) != -1)
    {
        if(cooldown <= clock())
        {
            if(index == packet.size)
                break;
            
            int action = data[index] & 0b111;
            int value = data[index] >> 3 & 0b111;
            
            switch(action)
            {
                case 0:
                    ++value;
                    println("%zu) moving %u units forward", index + 1, value);
                    motors_set_speed(MOTOR_SPEED, MOTOR_SPEED);
                    cooldown = clock() + MOVE_COOLDOWN * value;
                    break;
                
                case 1:
                    ++value;
                    println("%zu) moving %u units backward", index + 1, value);
                    motors_set_speed(-MOTOR_SPEED, -MOTOR_SPEED);
                    cooldown = clock() + MOVE_COOLDOWN * value;
                    break;
                
                case 2:
                    ++value;
                    println("%zu) rotating %u° left", index + 1, value * 45);
                    motors_set_speed(-MOTOR_SPEED, MOTOR_SPEED);
                    cooldown = clock() + ROTATE_COOLDOWN * value;
                    break;
                
                case 3:
                    ++value;
                    println("%zu) rotating %u° right", index + 1, value * 45);
                    motors_set_speed(MOTOR_SPEED, -MOTOR_SPEED);
                    cooldown = clock() + ROTATE_COOLDOWN * value;
                    break;
                
                case 4:
                    println("%zu) turning LED %u on", index + 1, value);
                    led_set(value, true);
                    break;
                
                case 5:
                    println("%zu) turning LED %u off", index + 1, value);
                    led_set(value, false);
                    break;
                
                case 6:
                    ++value;
                    println("%zu) waiting for %u seconds", index + 1, value);
                    motors_stop();
                    cooldown = clock() + CLOCKS_PER_SEC * value;
                    break;
                
                default:
                    println("unknown action");
                    break;
            }
            
            ++index;
        }
    }
    
    free(data);
    
    motors_stop();
    wb_robot_cleanup();
    
    return EXIT_SUCCESS;
}
