#include <stdio.h>
#include <stdbool.h>

#include <webots/robot.h>

#include "../util/motors.h"
#include "../util/ground_sensors.h"
#include "../util/consts.h"

#include "../common/barcode.h"
#include "../common/helpers.h"

#define MOTOR_SPEED     2.0

// rotate 135° left, turn LED 4 on, wait for 3 seconds
// 2, 2, 4, 4, 6, 2 (18, 36, 22)
#define TEST_STEPS 21
const color_t test_values[TEST_STEPS] =
        {white, black, white, white, black, white, white,
         white, black, white, white, black, white, black,
         black, white, black, white, black, black, black};

int main()
{
    wb_robot_init();
    motors_init();
    ground_init();
    
    motors_set_speed(MOTOR_SPEED, MOTOR_SPEED);
    
    const double time_per_step = sync();
    double step_start = now() + time_per_step / 2;
    
    unsigned step = 0;
    unsigned test_count = 0;
    
    while(wb_robot_step(TIME_STEP) != -1)
    {
        if(now() - step_start >= time_per_step)
        {
            step_start = now();
            color_t input = get_color();
            bool ok = input == test_values[step];
            
            println("%s = %s -> %s", col2str(input), col2str(test_values[step]), ok ? "ok" : "failed");
            
            ++step;
            if(ok)
                ++test_count;
        }
        
        if(step == TEST_STEPS)
            break;
    }
    
    motors_stop();
    
    println("%s (%d/%d)", test_count == TEST_STEPS ? "Test passed!" : "Test failed!", test_count, TEST_STEPS);
    println("motor speed: %.1f rad/s", MOTOR_SPEED);
    println("time/step: %.3f s", time_per_step);
    
    wb_robot_cleanup();
    
    return EXIT_SUCCESS;
}
