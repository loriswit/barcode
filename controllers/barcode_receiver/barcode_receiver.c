#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <webots/robot.h>

#include "../util/motors.h"
#include "../util/leds.h"
#include "../util/com.h"
#include "../util/consts.h"

#include "../common/helpers.h"

#define MOTOR_SPEED     2.0
#define DATA_MAX_SIZE   100

#define MOVE_COOLDOWN   (3 / MOTOR_SPEED)
#define ROTATE_COOLDOWN (1.17 / MOTOR_SPEED)

#define REMOTE_MULTIPLIER   0.5

/**
 * Executes a specific action.
 *
 * @param action The action numeric identifier
 * @param value The action parameter's value
 * @return The number of ticks needed to execute the action
 */
double execute_action(int action, int value);

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
    double cooldown = now();
    
    while(wb_robot_step(TIME_STEP) != -1)
    {
        while(cooldown <= now())
        {
            if(index == packet.size)
                goto end;
            
            int action = data[index] & 0b111;
            int value = data[index] >> 3 & 0b111;
            
            double duration = execute_action(action, value);
            if(wb_robot_get_mode() == WB_MODE_REMOTE_CONTROL)
                duration *= REMOTE_MULTIPLIER;
            
            cooldown = now() + duration;
            
            ++index;
        }
    }
    end:
    
    free(data);
    
    motors_stop();
    wb_robot_cleanup();
    
    return EXIT_SUCCESS;
}

double execute_action(int action, int value)
{
    switch(action)
    {
        case 0:
            ++value;
            println("Moving %u units forward", value);
            motors_set_speed(MOTOR_SPEED, MOTOR_SPEED);
            return MOVE_COOLDOWN * value;
        
        case 1:
            ++value;
            println("Moving %u units backward", value);
            motors_set_speed(-MOTOR_SPEED, -MOTOR_SPEED);
            return MOVE_COOLDOWN * value;
        
        case 2:
            ++value;
            println("Rotating %u° left", value * 45);
            motors_set_speed(-MOTOR_SPEED, MOTOR_SPEED);
            return ROTATE_COOLDOWN * value;
        
        case 3:
            ++value;
            println("Rotating %u° right", value * 45);
            motors_set_speed(MOTOR_SPEED, -MOTOR_SPEED);
            return ROTATE_COOLDOWN * value;
        
        case 4:
            println("Turning LED %u on", value);
            led_set((size_t) value, true);
            return 0;
        
        case 5:
            println("Turning LED %u off", value);
            led_set((size_t) value, false);
            return 0;
        
        case 6:
            ++value;
            println("Waiting for %u seconds", value);
            motors_stop();
            return value;
        
        default:
            println("Unknown action: %d (value: %d)", action, value);
            return 0;
    }
}
